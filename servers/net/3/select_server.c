#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <wait.h>
#include <sys/select.h>
#include <fcntl.h>
#include <errno.h>

#define BUF_SIZE 1024
#define SOCK_ITEM_LENGHT 1024

// 将 fd 设置为非阻塞
void setNOBlock(int fd)
{
    int status = fcntl(fd, F_GETFL);
    status = status | O_NONBLOCK;
    fcntl(fd, F_SETFL, status);
}

struct sock_item
{
    int fd;
    unsigned port;
    char ip[INET_ADDRSTRLEN];
    char rbuf[BUF_SIZE];
    char wbuf[BUF_SIZE];
    int rlen;
    int wlen;
    struct reactor *r;
    void (*close_cb)(struct sock_item *);
    void (*read_cb)(struct sock_item *);
    void (*write_cb)(struct sock_item *);
};

struct reactor
{
    fd_set rset;
    fd_set wset;
    int nfds;
    struct sock_item *socks[SOCK_ITEM_LENGHT];
    int runing;
};

void close_cb(struct sock_item *sock)
{
    printf("%s:%d close\n", sock->ip, sock->port);
    FD_CLR(sock->fd, &sock->r->rset);
    FD_CLR(sock->fd, &sock->r->wset);
    close(sock->fd);
    sock->r->socks[sock->fd] = NULL;
    free(sock);
}

void read_cb(struct sock_item *sock)
{
    sock->rlen = recv(sock->fd, sock->rbuf, BUF_SIZE - 1, 0);
    if (sock->rlen == 0) // 对方关闭
    {
        sock->close_cb(sock);
        return;
    }

    if (sock->rlen < 0)
    {
        sock->rlen = 0;

        if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
        {
            return;
        }

        // 发生错误关闭
        perror("recv error\n");
        sock->close_cb(sock);
        return;
    }

    sock->rbuf[sock->rlen + 1] = '\0';
    printf("rcv msg:%s\n", sock->rbuf);
    if (strstr(sock->rbuf, "quit"))
    {
        sock->close_cb(sock);
        return;
    }

    // 拷贝到写缓冲区
    memcpy(sock->wbuf, sock->rbuf, sock->rlen);
    sock->wlen = sock->rlen;

    // 取消读事件 设置写事件
    FD_CLR(sock->fd, &sock->r->rset);
    FD_SET(sock->fd, &sock->r->wset);
}

void write_cb(struct sock_item *sock)
{
    // 判断有写的数据可以写
    if (sock->wlen > 0)
    {
        int n = send(sock->fd, sock->wbuf, sock->wlen, 0);
        if (n < 0)
        {

            if (errno == EAGAIN || errno == EWOULDBLOCK)
                return;

            perror("send error\n");
            sock->close_cb(sock);
            return;
        }
    }

    sock->wlen = 0;
    // 取消写事件 注册读事件
    FD_CLR(sock->fd, &sock->r->wset);
    FD_SET(sock->fd, &sock->r->rset);
}

struct sock_item *create_sock_item(struct reactor *r, int fd, const char *ip, unsigned short port)
{
    struct sock_item *sock = (struct sock_item *)calloc(1, sizeof(struct sock_item));
    sock->r = r;
    sock->port = port;
    strcpy(sock->ip, ip);
    sock->fd = fd;

    sock->close_cb = &close_cb;
    sock->read_cb = &read_cb;
    sock->write_cb = &write_cb;
    setNOBlock(fd);
    // 添加读事件
    FD_SET(fd, &r->rset);
    return sock;
}

int init_listen_fd(unsigned short port)
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
    {
        perror("socket listen\n");
        return -1;
    }
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listenfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0)
    {
        perror("bind error\n");
        return -2;
    }

    if (listen(listenfd, 1024) < 0)
    {
        perror("listen error\n");
        return -2;
    }

    printf("listen port:%d listendfd:%d\n", port, listenfd);
    setNOBlock(listenfd);
    return listenfd;
}

struct reactor *create_reactor(int listenfd)
{
    struct reactor *r = (struct reactor *)calloc(1, sizeof(struct reactor));
    FD_ZERO(&r->rset);
    FD_ZERO(&r->wset);
    // 监听端口设置为可读事件 监听 accept
    FD_SET(listenfd, &r->rset);
    r->runing = 1;
    r->nfds = listenfd + 1;
    return r;
}

struct reactor *r;

void sig_handler(int signum)
{
    if (r == NULL)
        return;
    r->runing = 0;
}

int main(int argc, char *argv[])
{

    struct sigaction sig;
    sig.sa_handler = &sig_handler;
    sigemptyset(&sig.sa_mask);
    sigaddset(&sig.sa_mask, SIGTERM);
    sigaddset(&sig.sa_mask, SIGQUIT);
    sigaction(SIGINT, &sig, NULL);
    unsigned short port = 9999;

    if (argc > 1)
    {
        port = atoi(argv[1]);
    }

    int listenfd = init_listen_fd(port);
    if (listenfd < 0)
        return -1;

    r= create_reactor(listenfd);

    while (r != NULL && r->runing)
    {
        fd_set wset = r->wset;
        fd_set rset = r->rset;
        struct timeval tv;
        tv.tv_usec = 1000;
        int v = select(r->nfds, &rset, &wset, NULL, &tv);

        if (v > 0)
        {
            if (FD_ISSET(listenfd, &rset)) // listenfd 处理
            {
                struct sockaddr_in caddr = {0};
                socklen_t len = sizeof(struct sockaddr_in);
                int clientfd = accept(listenfd, (struct sockaddr *)&caddr, &len);
                unsigned cport = ntohs(caddr.sin_port);
                char *cip = inet_ntoa(caddr.sin_addr);
                if (clientfd < 0)
                {
                    perror("accept error\n");
                }
                else if (clientfd >= (SOCK_ITEM_LENGHT - 1)) // 超过该范围直接关闭
                {
                    printf("%s:%d clientfd more than %d\n", cip, cport, SOCK_ITEM_LENGHT - 1);
                    close(clientfd);
                }
                else
                {
                    printf("%s:%d connect server\n", cip, cport);
                    struct sock_item *sock = create_sock_item(r, clientfd, cip, cport);
                    r->socks[clientfd] = sock;
                    // 更新最大描述符
                    if (clientfd > (r->nfds - 1))
                    {
                        r->nfds = clientfd + 1;
                    }
                }
            }
            else
            {
                for (int i = listenfd + 1; i < r->nfds; i++)
                {
                    struct sock_item *sock = r->socks[i];
                    if (sock == NULL)
                        continue;
                    if (FD_ISSET(sock->fd, &rset))
                    {
                        sock->read_cb(sock);
                    }
                    else if (FD_ISSET(sock->fd, &wset))
                    {
                        sock->write_cb(sock);
                    }
                }
            }
        }
        else if (v < 0)
        {
            if (errno == EAGAIN)
            {
                continue;
            }
            else
            {
                perror("select error\n");
                r->runing = 0;
            }
        }
        else
        {
            // printf("select timeout\n");
        }
    }

    if (r != NULL)
    {
        for (int i = 0; i < SOCK_ITEM_LENGHT; i++)
        {
            struct sock_item *sock = r->socks[i];
            if (sock == NULL)
                break;
            sock->close_cb(sock);
        }

        free(r);
        r = NULL;
    }

    close(listenfd);
}