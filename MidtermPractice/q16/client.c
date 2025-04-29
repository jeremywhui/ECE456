#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer = {0};
    if (argc != 3) {
        printf("Usage: ./client <IP> <port>\n");
        exit(EXIT_FAILURE);
    }
    int port = argv
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family 
    return 0;
}
