#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

/* a function of printing a single character of the string at a time */
static void charatatime(char *str) ;
void err_sys(const char* x);

int main(void) 
{ 
    pid_t pid; 
    if ( (pid = fork()) < 0) 
        err_sys("fork error"); 
    else if (pid == 0) { 
        charatatime("Child process wants to print a long string.\n"); 
    } else { 
        charatatime("Parent process is doing something else.\n"); 
    } 
    exit(0); 
} 

static void charatatime(char *str) 
{ 
    char *ptr; 
    int  c;
    setbuf(stdout, NULL);       /* set unbuffered */ 
    for (ptr = str; c = *ptr++; ) 
        putc(c, stdout); 
} 

void err_sys(const char* x)
{
	perror(x);
	exit(1);
}
