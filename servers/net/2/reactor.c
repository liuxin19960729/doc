#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <string.h>
#define BUF_LENGTH 128
#define EVENTS_LENGHT 128
#define ITEM_LEIGHT 1024
#define PORT_COUNT 100

struct sock_item
{
    int fd;

    char *rbuffer;
    int rlenght;

    char *wbuffer;
    int wlenght;

    int event;
    struct reactor *r;

    void (*recv_cb)(struct sock_item *);
    void (*send_cb)(struct sock_item *);
    void (*accetp_cb)(struct sock_item *);

    void (*close_cb)(struct sock_item *);

    void (*setBlock_cb)(int, int);
};

struct event_block
{
    struct sock_item items[ITEM_LEIGHT];
    struct event_block *next;
};

struct reactor
{
    int epfd;
    int blkcnt;
    // 扩容数组
    struct event_block *block;
    int ccnt;
};

void close_cb(struct sock_item *sock)
{
    if (sock->rbuffer != NULL)
        free(sock->rbuffer);
    if (sock->wbuffer != NULL)
        free(sock->wbuffer);
    sock->wbuffer = NULL;
    sock->rbuffer = NULL;
    epoll_ctl(sock->r->epfd, EPOLL_CTL_DEL, sock->fd, NULL);
    close(sock->fd);
    sock->r->ccnt--;
    printf("clientfd:%d close ccnt:%d\n", sock->fd, sock->r->ccnt);
}

void setBlock_cb(int fd, int block)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (block)
    {
        /* code */
        flags &= ~O_NONBLOCK;
    }
    else
    {
        // 设置非BLOCK
        flags |= O_NONBLOCK;
    }
    fcntl(fd, F_SETFL, flags);
}

// 新增 event_block
int reactor_resize(struct reactor *r)
{
    if (r == NULL || r->block == NULL)
        return -1;

    struct event_block *cur = r->block;

    while (cur->next != NULL)
        cur = cur->next;

    cur->next = (struct event_block *)calloc(1, sizeof(struct event_block));

    if (cur->next == NULL)
        return -4;

    r->blkcnt++;
    return 0;
}

/**查找 socket_item */
struct sock_item *reactor_lookup(struct reactor *r, int fd)
{
    if (r == NULL || r->block == NULL)
        return NULL;
    if (fd < 0)
        return NULL;

    int blockIndex = fd / ITEM_LEIGHT;

    // 防止突然 fd 过大 预防意外情况
    while (blockIndex >= r->blkcnt)
    {
        // 扩容
        int flag = reactor_resize(r);
    }

    // 找到对应的block
    int i = 0;
    struct event_block *block = r->block;

    while (i < blockIndex)
    {
        i++;
        block = block->next;
    }

    return block->items + (fd % ITEM_LEIGHT);
}

int init_server(unsigned short port)
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    if (listenfd == -1)
    {
        perror("socket error\n");
        return -1;
    }

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    // INADDR_ANY
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
    {
        perror("bind error\n");
        return -2;
    }

    if (listen(listenfd, 10) == -1)
    {
        perror("listen error\n");
        return -1;
    }

    printf("bind port:%d\n", port);

    return listenfd;
}

int is_listen_fd(int fds[PORT_COUNT], int fd)
{
    int r = 0;

    for (int i = 0; i < PORT_COUNT && r != 1; i++)
    {
        r = fds[i] == fd ? 1 : 0;
    }

    return r;
}

int main(int argc, char *argv[])
{
    unsigned short port = 9999;

    if (argc > 1)
    {
        port = atoi(argv[1]);
    }

    int sockfds[PORT_COUNT] = {0};

    for (int i = 0; i < PORT_COUNT; i++)
    {
        sockfds[i] = init_server(port + i);
    }

    struct reactor *r = (struct reactor *)calloc(1, sizeof(struct reactor));
    if (r == NULL)
    {
        perror("calloc error\n");
        return -3;
    }

    // 动态数组分配
    r->blkcnt = 1;
    r->block = (struct event_block *)calloc(1, sizeof(struct event_block));

    if (r->block == NULL)
    {
        perror("calloc error\n");
        return -4;
    }

    r->epfd = epoll_create(1);
    struct epoll_event eventarr[EVENTS_LENGHT];

    // 绑定多个listen 端口到 epoll

    for (int i = 0; i < PORT_COUNT; i++)
    {
        struct epoll_event ev;
        ev.events = EPOLLIN;
        ev.data.fd = sockfds[i];
        epoll_ctl(r->epfd, EPOLL_CTL_ADD, sockfds[i], &ev);
    }

    for (;;)
    {
        int nready = epoll_wait(r->epfd, eventarr, EVENTS_LENGHT, -1);

        for (int i = 0; i < nready; i++)
        {
            struct epoll_event ev = eventarr[i];
            int clientfd = ev.data.fd;
            if (is_listen_fd(sockfds, clientfd) == 1)
            {
                int listenfd = clientfd;
                struct sockaddr_in clientaddr;
                socklen_t len = sizeof(clientaddr);
                // accept 阻塞函数
                clientfd = accept(listenfd, (struct sockaddr *)&clientaddr, &len);
                if (clientfd <= 0)
                {
                    perror("accept error\n");
                    continue;
                }
                struct sock_item *item = reactor_lookup(r, clientfd);
                r->ccnt++;
                unsigned short cport = ntohs(clientaddr.sin_port);
                char *ip = inet_ntoa(clientaddr.sin_addr);
                printf("client:  clientfd:%d  address: %s:%d connect server\n", clientfd, ip, cport);
                // 添加clientfd 到epoll
                struct epoll_event ev;
                ev.events = EPOLLIN;
                ev.data.fd = clientfd;
                epoll_ctl(r->epfd, EPOLL_CTL_ADD, clientfd, &ev);
#if 0
                r->items[clientfd].fd = clientfd;
                r->items[clientfd].rbuffer = calloc(1, BUF_LENGTH);
                r->items[clientfd].rlenght = 0;
                r->items[clientfd].wbuffer = calloc(1, BUF_LENGTH);
                r->items[clientfd].wlenght = 0;
                r->items[clientfd].event = EPOLLIN;
                r->items[clientfd].r = r;
                r->items[clientfd].close_cb = &close_cb;
                r->items[clientfd].setBlock_cb = &setBlock_cb;
#else

                item->fd = clientfd;
                item->rbuffer = calloc(1, BUF_LENGTH);
                item->rlenght = 0;
                item->wbuffer = calloc(1, BUF_LENGTH);
                item->wlenght = 0;
                item->event = EPOLLIN;
                item->r = r;
                item->close_cb = &close_cb;
                item->setBlock_cb = &setBlock_cb;
#endif
            }
            else if (ev.events & EPOLLIN)
            {
                struct sock_item *item = reactor_lookup(r, clientfd);
                char *rbuffer = item->rbuffer;
                char *wbuffer = item->wbuffer;
                int n = recv(clientfd, rbuffer, BUF_LENGTH, 0);
                if (n <= 0) // 断开连接
                {
                    if (n < 0)
                        perror("recv error\n");

                    item->close_cb(item);

                    continue;
                }

                memcpy(wbuffer, rbuffer, BUF_LENGTH);

                printf("recv:msg:%s,len:%d\n", rbuffer, n);

                if (strstr(rbuffer, "quit") != NULL)
                {
                    item->close_cb(item);
                    continue;
                }

                // 修改clientfd 值监听写事件
                struct epoll_event ev;
                ev.events = EPOLLOUT;
                ev.data.fd = clientfd;
                epoll_ctl(r->epfd, EPOLL_CTL_MOD, clientfd, &ev);
            }
            else if (ev.events & EPOLLOUT)
            {
                struct sock_item *item = reactor_lookup(r, clientfd);
                char *wbuffer = item->wbuffer;
                send(clientfd, wbuffer, BUF_LENGTH, 0);

                struct epoll_event ev;
                ev.events = EPOLLIN;
                ev.data.fd = clientfd;
                epoll_ctl(r->epfd, EPOLL_CTL_MOD, clientfd, &ev);
            }
        }
    }

    for (int i = 0; i < PORT_COUNT; i++)
    {
        close(sockfds[i]);
    }

    return 0;
}