#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>

#define PORT 8080
#define MAXLINE 4096


static void sig_alrm(int);

void
echo_client(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen)
{
    int n;
    char    sendline[MAXLINE], recvline[MAXLINE + 1];
    struct sigaction action;
    if (sigaction(SIGALRM, NULL, &action) < 0) {
        perror("sgiaction get");
        exit(1);
    }
    action.sa_handler = sig_alrm;
    action.sa_flags &= ~SA_RESTART;
    if (sigaction(SIGALRM, &action, NULL) < 0) {
        perror("sgiaction get");
        exit(1);
    }
//    signal(SIGALRM, sig_alrm);

    while (fgets(sendline, MAXLINE, fp) != NULL) {

        sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);

        alarm(5);
        if ((n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL)) < 0){
            if (errno == EINTR)
                fprintf(stderr, "socket timeout\n");
            else
                perror("recvfrom error");
        }else{
            alarm(0);
            recvline[n] = 0;    /* null terminate */
            fputs(recvline, stdout);
        }
    }
}

static void sig_alrm(int signo)
{
    return; /* just interrupt the recvfrom() */
}

int main(int argc, char const *argv[])
{
    int sockfd;
    struct sockaddr_in serv_addr;

    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    // Convert IP addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }


    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    printf("The UDP client is on.\n");
    echo_client(stdin, sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    exit(0);
}



