#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <fcntl.h>


#define MAXLINE 4096            /* max line length */
/*
 * Default file access permissions for new files.
 */
#define FILE_MODE   (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
/*
 * Print a message and return to caller.
 * Caller specifies "errnoflag".
 */
static void err_doit(int errnoflag, int error, const char *fmt, va_list ap)
{
    char    buf[MAXLINE];
   vsnprintf(buf, MAXLINE, fmt, ap);
   if (errnoflag)
       snprintf(buf+strlen(buf), MAXLINE-strlen(buf), ": %s",
         strerror(error));
   strcat(buf, "\n");
   fflush(stdout);     /* in case stdout and stderr are the same */
   fputs(buf, stderr);
   fflush(NULL);       /* flushes all stdio output streams */
}

/*
 * Fatal error related to a system call.
 * Print a message and terminate.
 */
void err_sys(const char *fmt, ...)
{
    va_list     ap;

    va_start(ap, fmt);
    err_doit(1, errno, fmt, ap);
    va_end(ap);
    exit(1);
}

char    buf1[] = "abcdefghij";
char    buf2[] = "ABCDEFGHIJ";

int
main(void)
{
    // create a file descriptor
    int fd;
    // create a file named file.hole to the file descriptor
    // If an error occurs, call err_sys
    if ((fd = creat("file.hole", FILE_MODE)) < 0)
        err_sys("creat error");
    // Write the contents of buf1 to fd
    // If an error occurs, call err_sys
    if (write(fd, buf1, 10) != 10)
        err_sys("write buf1 error");
    /* offset now = 10 */
    // Skip to position 40 in the file
    if (lseek(fd, 40, SEEK_SET) < 0)
        err_sys("lseek error");

    /* offset now = 40 */
    // Write the contents of buf2 to fd
    if (write(fd, buf2, 10) != 10)
        err_sys("write buf2 error");
    /* offset now = 50 */
    // exit 0
}
