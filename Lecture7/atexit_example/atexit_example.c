#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

static void my_exit1(void), my_exit2(void); 
void err_sys(const char* x);

int main(void) 
{ 
    if (atexit(my_exit2) != 0) 
        err_sys("can't register my_exit2"); 

    if (atexit(my_exit1) != 0) 
        err_sys("can't register my_exit1"); 

    if (atexit(my_exit1) != 0) 
        err_sys("can't register my_exit1"); 

    printf("main is done\n"); 
    return(0); 
} 

static void my_exit1(void) 
{ 
    printf("first exit handler\n"); 
} 

static void my_exit2(void) 
{ 
    printf("second exit handler\n"); 
} 

void err_sys(const char* x)
{
	perror(x);
	exit(1);
}
