#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// This program receives multicast messages
int main(int argc, char *argv[])
{
    struct sockaddr_in addr;
    int fd, nbytes;
	socklen_t addrlen;
    struct ip_mreq mreq; //structure provides multicast group information for IPv4 addresses
    char msgbuf[256]; //message buffer

	printf("The multicast client is on. \n");
    /* create an ordinary UDP socket */
    if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0) {
	    perror("socket");
	    exit(1);
    }

    /* set up destination address */
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=htonl(INADDR_ANY); /* differs from sender */
    addr.sin_port=htons(10000);
     
    /* bind to receiver's address */
    if (bind(fd,(struct sockaddr *) &addr,sizeof(addr)) < 0) {
	    perror("bind");
	    exit(1);
    }
     
    /* use setsockopt() to request joining a 255.0.0.37 group */
    mreq.imr_multiaddr.s_addr=inet_addr("225.0.0.37");
    mreq.imr_interface.s_addr=htonl(INADDR_ANY);
    if (setsockopt(fd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) < 0) {
	    perror("setsockopt");
	    exit(1);
    }

    /* now just enter a read-print loop */
    while (1) {
	    addrlen=sizeof(addr);
	    if ((nbytes=recvfrom(fd,msgbuf,256,0,
			       (struct sockaddr *) &addr,&addrlen)) < 0) {
	       perror("recvfrom");
	       exit(1);
	    }
	    puts(msgbuf);
    }
}
