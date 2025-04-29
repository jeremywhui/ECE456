#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>
#include <stdio.h>
#include <unistd.h>

// receive broadcase messages
int main() {

    int sockfd = socket(PF_INET, SOCK_DGRAM, 0);  
    printf("The broadcast receiver is started.\n");
    struct sockaddr_in dest;
    bzero(&dest, sizeof(dest));
    dest.sin_family = PF_INET;
    dest.sin_addr.s_addr=INADDR_ANY;
    dest.sin_port = htons(10000);

    if( bind(sockfd, (struct sockaddr*) &dest, sizeof(dest)) < 0) {
        perror("bind");
        return 1;
    }

    int on=1;
    //set socket option to be broadcast
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));   
  
    while(1) { 
        char recvmsg[100];
        struct sockaddr_in src;
        socklen_t size=sizeof(src);
        recvfrom(sockfd, recvmsg, sizeof(recvmsg), 0, (struct sockaddr*) &src, &size);
        printf("Received: %s" ,recvmsg);
    }  
}
