#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
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


void
echo_client(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen)
{
    int n;
    char    sendline[MAXLINE], recvline[MAXLINE + 1];

    while (fgets(sendline, MAXLINE, fp) != NULL) {

        sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);

        if (readable_timeout(sockfd, 5) == 0){
            fprintf(stderr, "socket timeout\n");
        }else{
            n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
            recvline[n] = 0;    /* null terminate */
            fputs(recvline, stdout);
        }
        
    }
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



