#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
/* Spawn a child process running a new program. PROGRAM is the name
of the program to run; the path will be searched for this program.
ARG_LIST is a NULL-terminated list of character strings to be
passed as the program's argument list. Returns the process ID of
the spawned process. */
int spawn(char* program, char** arg_list)
{
    pid_t child_pid;
    /* Duplicate this process. */
    child_pid = fork();
    if (child_pid != 0)
        /* This is the parent process. */
        return child_pid;
    else {
        /* Now execute PROGRAM, searching for it in the path. */
        execvp(program, arg_list);
        /* The execvp function returns only if an error occurs. */
        fprintf(stderr, "an error occurred in execvp\n");
        abort();
    }
}

int main()
{
    int child_status;
    /* The argument list to pass to the "ls" command. */
    char* arg_list[] = {
        "ls", /* argv[0], the name of the program. */
        "-l",
        "/",
        NULL /* The argument list must end with a NULL. */
    };
    printf("print in parent process before calling spawn.\n");
    /* Spawn a child process running the "ls" command. Ignore the
    returned child process ID. */
    spawn("ls", arg_list);

    /*wait for the child process to complete. */
    wait(&child_status);
    if(WIFEXITED(child_status))
        printf("the child process exited normally, with exit code %d.\n",WEXITSTATUS(child_status));
    else
        printf("the child process exited abnormally.\n");

    printf("done with the main program.\n");
    return 0;
}
