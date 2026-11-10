#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <wait.h>

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

// 信号处理函数
// wait 并不能完全处理完僵尸进程如果同时子进程退出就会有一些子进程处理不了
void sig_handler(int signum)
{
    int status = 0;
    pid_t pid;

    while ((pid= waitpid(-1,&status,WNOHANG))>0)
    {
        printf("pid %d exit status:%d\n", pid, status);
    }
}

int main(int argc, char *argv[])
{

    struct sigaction sig = {0};

    sig.sa_handler = &sig_handler;
    sig.sa_flags = SA_RESTART;

    // 清空屏蔽信号集合
    sigemptyset(&sig.sa_mask);
    sigaddset(&sig.sa_mask, SIGTERM);
    sigaddset(&sig.sa_mask, SIGQUIT);

    sigaction(SIGCHLD, &sig, NULL);

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

    printf("listen port:%d listendfd:%d\n", port, listenfd);

    for (;;)
    {
        struct sockaddr_in caddr = {0};
        socklen_t len = sizeof(struct sockaddr_in);
        int clientfd = accept(listenfd, (struct sockaddr *)&caddr, &len);
        if (clientfd < 0)
        {

            perror("accept error\n");
            continue;
        }

        unsigned cport = ntohs(caddr.sin_port);
        char *cip = inet_ntoa(caddr.sin_addr);

        // 多进程服务器
        pid_t cpid = fork();
        printf("cpid:%d\n", cpid);
        if (cpid == 0)
        {
            // 忽略
            signal(SIGCHLD,SIG_DFL);
            char *ccip = (char *)calloc(1, strlen(cip) + 1);
            strcpy(ccip, cip);
            close(listenfd);
            printf("client %s:%d connect server clientfd:%d\n", ccip, cport, clientfd);
            doit(clientfd, cport, ccip);
            close(clientfd);
            free(ccip);
            printf("pid:%d quit\n", getpid());
            exit(0);
        }
        else
        {
            if (cpid < 0)
                perror("fork error\n");
            close(clientfd);
        }
    }

    close(listenfd);
    printf("finished\n");
}