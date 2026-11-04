#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdlib.h>

#define BUF_LENGTH 128
void setFDBlock(int fd, int block)
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

void *worker(void *argv)
{
    int clientfd = (long)argv;
    unsigned char buf[BUF_LENGTH] = {0};
    for (;;)
    {
        int ret = recv(clientfd, buf, BUF_LENGTH, 0);

        if (ret == 0)
        { // ret 0 断开连接

            printf("cliendfd:%d close\n", clientfd);
            break;
        }
        printf("buf:%s ret:%d\n", buf, ret);
        send(clientfd, buf, ret, 0);
    }

    close(clientfd);
}

int main(int argc, char *argv[])
{
    unsigned short port = 9999;

    if (argc > 1)
    {
        port = atoi(argv[1]);
    }

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

    printf("bind port:%d\n",port);
#if 0
    struct sockaddr_in clientaddr;
    socklen_t len = sizeof(clientaddr);
    // accept 阻塞函数
    int clientfd = accept(listenfd, (struct sockaddr *)&clientaddr, &len);
    unsigned char buf[BUF_LENGTH] = {0};
    for (;;)
    {
        int ret = recv(clientfd, buf, BUF_LENGTH, 0);

        if (ret <= 0)
        { // ret 0 断开连接

            break;
            
        }

        printf("buf:%s ret:%d\n", buf, ret);
        send(clientfd, buf, ret, 0);
    }
#elif 0
    // 一请求一线程
    for (;;)
    {

        struct sockaddr_in clientaddr;
        socklen_t len = sizeof(clientaddr);
        // accept 阻塞函数
        int clientfd = accept(listenfd, (struct sockaddr *)&clientaddr, &len);
        if (clientfd < 0)
        {
            perror("accept error\n");
            continue;
        }
        pthread_t pid;
        pthread_create(&pid, NULL, &worker, (void *)(long)clientfd);
    }

#else
    // IO 多路复用

    fd_set wfds, rfds, rset, wset;
    FD_ZERO(&rfds);
    FD_SET(listenfd, &rfds);

    FD_ZERO(&wfds);
    int maxfd = listenfd;
    unsigned char buf[BUF_LENGTH] = {0};
    int ret = 0;
    for (;;)
    {
        rset = rfds;
        wset = wfds;
        int nready = select(maxfd + 1, &rset, &wset, NULL, NULL);

        if (FD_ISSET(listenfd, &rset))
        {
            struct sockaddr_in clientaddr;
            socklen_t len = sizeof(clientaddr);
            // accept 阻塞函数
            int clientfd = accept(listenfd, (struct sockaddr *)&clientaddr, &len);

            FD_SET(clientfd, &rfds);

            if (clientfd > maxfd)
                maxfd = clientfd;

            unsigned short cport = ntohs(clientaddr.sin_port);
            char *ip = inet_ntoa(clientaddr.sin_addr);
            printf("client:%s:%d connect server\n", ip, cport);
        }

        for (int i = listenfd + 1; i <= maxfd; i++)
        {
            if (FD_ISSET(i, &rset))
            {

                ret = recv(i, buf, BUF_LENGTH, 0);

                // ret 0 断开连接
                // 关闭 fd
                // 清空 fd 在 rfds 和 wfds 中的数据
                if (ret == 0)
                {

                    printf("cliendfd:%d close\n", i);
                    close(i);
                    FD_CLR(i, &rfds);
                    FD_CLR(i, &wfds);
                    continue;
                }
                printf("buf:%s ret:%d\n", buf, ret);
                FD_SET(i, &wfds);
            }

            if (FD_ISSET(i, &wset))
            {
                if (ret > 0)
                {
                    ret = send(i, buf, ret, 0);
                    ret = 0;
                }
                FD_CLR(i, &wfds);
            }
        }
    }

#endif

    close(listenfd);
    return 0;
}