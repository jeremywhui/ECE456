#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define LISTENQ 1024

void err_exit() {
  perror("myserver");
  exit(1);
}

int main() {
  int sockfd;
  if((sockfd=socket(AF_INET, SOCK_STREAM, 0)) < 0)
    err_exit();
  
  struct sockaddr_in serveraddr;
  bzero(&serveraddr,sizeof(serveraddr));
  serveraddr.sin_family=AF_INET;
  serveraddr.sin_addr.s_addr=htonl(INADDR_ANY);
  serveraddr.sin_port=htons(8080);

  if(bind(sockfd, (struct sockaddr*) &serveraddr, sizeof(serveraddr)) < 0)
    err_exit();
  
  if(listen(sockfd, LISTENQ) <0)
    err_exit();
  
  for(;;) {
    int connfd;
    if ((connfd = accept(sockfd, 0, 0)) < 0)
      err_exit();
    printf("Socket connected\n");
    char response[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<HTML><BODY>Hello World from EE456!!!</BODY></HTML>\r\n";
    if (write(connfd, response, sizeof(response)) < 0)
      err_exit();
    printf("HTTP response sent\n");
    close(connfd);
  }
}
