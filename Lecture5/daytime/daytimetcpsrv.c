#include 	<sys/socket.h>
#include 	<sys/types.h>
#include	<time.h>
#include	<string.h>
#include 	<netinet/in.h>
#include 	<stdio.h>
#include 	<unistd.h>

#define	MAXLINE		4096	/* max text line length */
#define	LISTENQ		1024	/* 2nd argument to listen() */

typedef struct sockaddr SA;

int
main(int argc, char **argv)
{
	int					listenfd, connfd;
	struct sockaddr_in	servaddr;
	char				buff[MAXLINE];
	time_t				ticks;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(13);	/* daytime server */

	bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	listen(listenfd, LISTENQ);

	for ( ; ; ) {
		connfd = accept(listenfd, (SA *) NULL, NULL);

		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		write(connfd, buff, strlen(buff));

		close(connfd);
	}
}
