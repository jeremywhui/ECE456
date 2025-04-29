#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_FILE_C2S "/tmp/fifo_c2s"
#define FIFO_FILE_S2C "/tmp/fifo_s2c"

// This program open named pipes, write to one and read from the other.
int main (int argc, char *argv[])
{
    int fd1, fd2; 
    char buf[1024];
    char *message = "Hello from client";
    int size, num=1;

    if (argc > 1) {
        num = atoi(argv[1]);
    }
    // open FIFOs
    if ((fd1 = open(FIFO_FILE_C2S, O_WRONLY)) == 0) {
        perror("open");
        exit (1);
    }
    if ((fd2 = open(FIFO_FILE_S2C, O_RDONLY)) == 0) {
        perror("open");
        exit (1);
    }
    for (int i=0; i<num; i++) {
        write(fd1, message, strlen(message)+1);
        printf("send to server: %s\n", message);
        size = read(fd2, buf, 1024);
        buf[size] = 0;
        printf("receive: %s\n", buf);
    }
    close(fd1);
    close(fd2);
	return 0;
}
