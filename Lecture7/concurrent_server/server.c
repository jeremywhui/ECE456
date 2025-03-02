#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#define PORT 8080
int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    int pid;
    char *response = "Response from server";
      
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl (INADDR_ANY);
    address.sin_port = htons( PORT );
      
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 1024) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while(1){
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        /* Create a new process to cope with each new client request */
        if ( (pid = fork()) == 0) {    /* child process */
            close(server_fd);    /* close listening socket */
            /* process the request */
            valread = read( new_socket , buffer, 1024);    // recevie the msg
            printf("%s\n",buffer );
            sleep(2);               // simulate server processing time 
            write(new_socket, response, strlen(response)); //response from server
            printf("Response msg sent\n");
            exit(0);
        }
        
        close(new_socket);  /* parent closes connected socket */
        printf("after close socket\n");
    }
    
    return 0;
}
