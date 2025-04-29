#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

// This program creates pipes and use them for IPC
int main ()
{
	int fds[2], fds2[2];
    char buf[1024];
    int status, size, num=0;

	// Create pipe: file descriptors for the two ends of the pipe are in fds 
	pipe (fds);
	pipe (fds2);
	// Fork a child process 
	if (fork () == 0) { 
        // child: in our copy close the read end of the file descriptor 
		close (fds[0]);
        char hello[] = "Hello, World!";
        // write messages to pipe
        for (int i=0; i<5; i++) {
            write (fds[1], hello, sizeof(hello));
            sleep(1);
        }
		close (fds[1]);
		// read response back using the 2nd pipe
		close (fds2[1]);
        read(fds2[0], buf, sizeof(buf));
		printf("----------\n");
        printf("Response: %s\n", buf);
		close (fds2[0]);
	}
	else { // parent: close our copy of the write end of the file descriptor
		close (fds[1]);
        while ((size = read(fds[0], buf, sizeof(buf))) != 0) {
            if (size < 0) {
                perror("read");
                exit(1);
            }
            printf("from child: %s\n", buf);
            num++;
        }
		close (fds[0]);
		// Use the second pipe to write response 
		close (fds2[0]);
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%d messages received", num);
		write (fds2[1], buf, strlen(buf)+1);
		close (fds2[1]);
        wait(&status);
	}
	return 0;
}
