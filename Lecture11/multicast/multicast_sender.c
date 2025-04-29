#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>
#include <stdio.h>
#include <unistd.h>

// This program sends multicast message periodically
int main() {
    int sockfd = socket(PF_INET, SOCK_DGRAM, 0);  

    char msg[] = "This is a multicast message.\n";
    printf("The multicast sender is on.\n");
    struct sockaddr_in dest;
    bzero(&dest, sizeof(dest));
    dest.sin_family = PF_INET;
    //Use the multicast address 225.0.0.37 to multicast a message
    inet_pton(PF_INET,"225.0.0.37",&(dest.sin_addr.s_addr)); 
    dest.sin_port = htons(10000);

    while(1) {
        printf("The multicast sender sends: This is a multicast message.\n");  
        sendto(sockfd, msg, sizeof(msg), 0, (struct sockaddr*) &dest, sizeof(dest));
        sleep(5);
    }
}
