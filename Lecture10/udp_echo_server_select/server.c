#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#define PORT 8080
#define MAXLINE 4096

int
readable_timeout(int fd, int sec){
    fd_set rset;
    struct timeval tv;
    FD_ZERO(&rset);
    FD_SET(fd, &rset);
    tv.tv_sec = sec;
    tv.tv_usec = 0;
    return (select(fd + 1, &rset, NULL, NULL, &tv));
    /* > 0 if descriptor is readable */

}

void udp_echo(int sockfd, struct sockaddr *pcliaddr, socklen_t clilen)
{
    int         n;
    socklen_t   len;
    char        msg[MAXLINE+1];

    for ( ; ; ) {
        len = clilen;
        if (readable_timeout(sockfd, 5) == 0){
            fprintf(stderr, "socket timeout\n");
        }else{
            n = recvfrom(sockfd, msg, MAXLINE, 0, pcliaddr, &len);
            //receive message from the client
            sendto(sockfd, msg, n, 0, pcliaddr, len);
            //send message back to the client
            msg[n] = 0;    /* null terminate */
            printf("Received and sent: %s\n", msg); //print out the message
        }
        
    }
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


