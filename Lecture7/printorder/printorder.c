#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
int glob = 6; /* external variable in initialized data */

char buf[] = "write to stdout\n";

void err_sys(const char *x)
{
	perror(x);
	exit(1);
}

int main(void)
{

	int var; /* automatic variable on the stack */
	pid_t pid;

	var = 88;
	if (write(STDOUT_FILENO, buf, sizeof(buf) - 1) != sizeof(buf) - 1)
		err_sys("write error");

	printf("before fork\n"); /* we don't flush stdout */

	if ((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid == 0)
	{	/* child */
		/* modify variables */
		printf("the child process starts\n");
		glob++;
		var++;
	}
	else
	{
		sleep(2); /* parent */
		printf("this is the parent process\n");
	}

	printf("pid = %d, glob = %d, var = %d\n", getpid(), glob, var);

	exit(0);
}
