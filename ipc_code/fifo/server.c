#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_FILE_C2S "/tmp/fifo_c2s"
#define FIFO_FILE_S2C "/tmp/fifo_s2c"
// This program creates named pipes, reads message and writes response.
int main ()
{
    int fd1, fd2;
    char buf[1024];
    char response[] = "Hello from server";
    int size; 
	// Create FIFO for server read 
    if (mkfifo(FIFO_FILE_C2S, S_IFIFO|0640) < 0) {
        if (errno != EEXIST )
        {
            perror("mkfifo");
            exit(1);
        }
    }
	// Create FIFO for server write
    if (mkfifo(FIFO_FILE_S2C, S_IFIFO|0640) < 0) {
        if (errno != EEXIST )
        {
            perror("mkfifo");
            exit(1);
        }
    }
    // open fifos
    if ((fd1 = open(FIFO_FILE_C2S, O_RDONLY)) == 0) {
        perror("open");
        exit (1);
    }
    if ((fd2 = open(FIFO_FILE_S2C, O_WRONLY)) == 0) {
        perror("open");
        exit (1);
    }
    for (;;) {
        size = read(fd1, buf, 1024);
        if (size == 0) {
            printf("Peer closed FIFO. If not reopen, further read may return size 0, exit\n");
            break;
        }
        printf("receive: %s\n", buf);
        write(fd2, response, sizeof(response));
        sleep(1);
    }
    close(fd1);
    close(fd2);
    unlink(FIFO_FILE_C2S);
    unlink(FIFO_FILE_S2C);
	return 0;
}
