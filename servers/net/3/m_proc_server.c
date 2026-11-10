#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 1024

void doit(int clienfd, unsigned short cport, char *cip)
{
    char buf[BUF_SIZE] = {0};
    for (;;)
    {
        int n = recv(clienfd, buf, BUF_SIZE - 1, 0);
        if (n == 0)
        {
            printf("client close %s:%d \n", cip, cport);
            break;
        }

        buf[n + 1] = '\0';

        if (strstr(buf, "quit") != NULL)
        {
            printf("%s:%d quit\n", cip, cport);
            break;
        }

        printf("%s\n", buf);
        write(clienfd, buf, n);
    }
}

int main(int argc, char *argv[])
{

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
    {
        perror("socket listen\n");
        return -1;
    }

    unsigned short port = 9999;

    if (argc > 1)
    {
        port = atoi(argv[1]);
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

    printf("listen port:%d\n", port);

    for (;;)
    {
        struct sockaddr_in caddr = {0};
        socklen_t len = sizeof(struct sockaddr_in);
        int clientfd = accept(listenfd, (struct sockaddr *)&caddr, &len);
        if (clientfd < 0)
        {
            continue;
        }

        unsigned cport = ntohs(caddr.sin_port);
        char *cip = inet_ntoa(caddr.sin_addr);

        printf("client %s:%d connect server\n", cip, cport);

        // 多进程服务器
        if (fork() == 0)
        {
            char *ccip = (char *)calloc(1, strlen(ccip) + 1);
            strcpy(ccip, cip);
            close(listenfd);
            doit(clientfd, cport, ccip);
            close(clientfd);
            free(ccip);
            printf("pid:%d quit\n",getpid());
            exit(0);
        }
        else
        {
            close(clientfd);
        }
    }

    close(listenfd);
}