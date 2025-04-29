#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>
#include <stdio.h>
#include <unistd.h>

// This program sends broadcast message periodically
int main() {
    // create UDP socket
    int sockfd = socket(PF_INET, SOCK_DGRAM, 0);  

    char msg[] = "This is a broadcast message.\n";
 
    struct sockaddr_in dest;
    bzero(&dest, sizeof(dest));
    dest.sin_family = PF_INET;
    inet_pton(PF_INET,"255.255.255.255",&(dest.sin_addr.s_addr)); 
    dest.sin_port = htons(10000);

    int on=1;
    //Set the socket option to be broadcast
    setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));   
    printf("The broadcast sender is on.\n");
 
    //Broadcast a message every 5 seconds
    while(1) { 
        printf("The broadcast sender sends: This is a broadcast message.\n");
        sendto(sockfd, msg, sizeof(msg), 0, (struct sockaddr*) &dest, sizeof(dest));
        sleep(5);
    }  
}
