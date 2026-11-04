#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>

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

        if (ret <= 0)
        { // ret 0 断开连接

            break;
        }
        printf("buf:%s ret:%d\n", buf, ret);
        send(clientfd, buf, ret, 0);
    }

    close(clientfd);
}

int main()
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
    servaddr.sin_port = htons(9999);

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
#else
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

#endif

    close(listenfd);
    return 0;
}