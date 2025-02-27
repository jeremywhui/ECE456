#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#define PORT 8080

int         /* Read "n" bytes from a descriptor. */
readnbytes(int fd, void *vptr, int n) {
    int i=0;  // for debug
    int nleft; // number of bytes left to be read
    int nread; // number of bytes have been read
    char    *ptr;

    ptr = vptr;
    nleft = n;
    while (nleft > 0) {
        if ( (nread = read(fd, ptr, nleft)) < 0) { 
            if (errno == EINTR){     /* Interrupted by signal handler*/
                nread = 0;     /* and call read() again */
                printf("Read Failure\n");
            }
            else
                return(-1);
        } else if (nread == 0)
            break;              /* EOF */
        printf("The %dth read acutally reads %d bytes.\n", i, nread); //debug
        nleft = nleft - nread;
        ptr   = ptr + nread;
        i = i+1;
    }
    return(n - nleft);      /* return >= 0 */
}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
      
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

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
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    int count = 0;
    FILE* fp=fopen("receive.txt","w+");
    while((valread = readnbytes(new_socket, buffer, 1024))>0){
        fwrite(buffer,1,valread,fp);
        count += valread;
    }
    printf("readnbytes function is called\n");
    printf("%d bytes of data received\n", count);
    close(new_socket);
    fclose(fp);
}
