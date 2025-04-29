#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#define PORT 8080
#define MAXLINE 4096

static void sig_alrm(int);

void udp_echo(int sockfd, struct sockaddr *pcliaddr, socklen_t clilen)
{
    int         n;
    socklen_t   len;
    char        msg[MAXLINE+1];
    struct sigaction action;
    if (sigaction(SIGALRM, NULL, &action) < 0) {
        perror("sigaction get");
        exit(1);
    }
    action.sa_handler = sig_alrm;
    action.sa_flags &= ~SA_RESTART;
    if (sigaction(SIGALRM, &action, NULL) < 0) {
        perror("sigaction");
        exit(1);
    }
    //siginterrupt(SIGALRM, 1);
    //signal(SIGALRM, sig_alrm);

    for ( ; ; ) {
        len = clilen;

        alarm(5);
        //receive message from the client
        if((n = recvfrom(sockfd, msg, MAXLINE, 0, pcliaddr, &len)) < 0){
            if (errno == EINTR)
                fprintf(stderr, "socket timeout\n");
            else
                perror("recvfrom error");
        }else{
            alarm(0);
            sendto(sockfd, msg, n, 0, pcliaddr, len);
            //send message back to the client
            msg[n] = 0;    /* null terminate */
            printf("Received and sent: %s\n", msg); //print out the message
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
    struct sockaddr_in  servaddr, cliaddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(PORT);

    bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    printf("The UDP server is on.\n");
    udp_echo(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr));

}


