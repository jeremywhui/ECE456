#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

/*The following example demonstrates how to place a lock on 10 bytes from the offset 100 in a file and remove it afterwards*/

int
main(int argc, char *argv[])
{
    int fd;
    struct flock fl;
    char buf[20];

    printf ("Opening file.\n");
    fd = open("testfile.txt", O_RDWR);
    if (fd == -1){
        perror("File open failed");
        exit(EXIT_FAILURE);
    }


    /* Make a non-blocking request to place a write lock
       on on 10 bytes from the offset 100 in a file */
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 100;
    fl.l_len = 10;

    printf ("Locking the file.\n");
    if (fcntl(fd, F_SETLK, &fl) == -1) {
        if (errno == EACCES || errno == EAGAIN) {
            printf("Already locked by another process\n");
            /* We can't get the lock at the moment */
        } else {
            perror("Handle unexpected error");
        }
    } else {
        printf ("Lock was granted.\n");
        /* Perform I/O on bytes 100 to 109 of file */
        lseek(fd, 100, SEEK_SET); /* seek to offset 100 from the start */
        read(fd, buf, 10); /* read from 100 to 109 bytes */
        buf[10] = '\0';//terminates the 10-byte buf string
        printf("The 100-109 bytes in the file are: %s\n", buf);

        /* Unlock the locked bytes */
        fl.l_type = F_UNLCK;
        fl.l_whence = SEEK_SET;
        fl.l_start = 100;
        fl.l_len = 10;
        printf ("Unlocking the file.\n");
        if (fcntl(fd, F_SETLK, &fl) == -1){
            perror("Handle unexpected error");
        }else{
            printf ("Unlock was granted.\n");
        }
    }
    exit(EXIT_SUCCESS);
} /* main */