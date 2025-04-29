#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#define PORT 8080
#define MAXLINE 4096
#define TIMEOUT 5

void udp_echo(int sockfd, struct sockaddr *pcliaddr, socklen_t clilen)
{
    int         n;
    socklen_t   len;
    char        msg[MAXLINE+1];

    // set socket timeout
    struct timeval tv;
    tv.tv_sec = TIMEOUT;
    tv.tv_usec = 0;
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv))!=0) {
		perror("setsockopt");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

    for ( ; ; ) {
        len = clilen;
        if ((n = recvfrom(sockfd, msg, MAXLINE, 0, pcliaddr, &len)) < 0) {
			perror("recvfrom");
		}
		else {
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


