#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h>

// Program demonstrating the use of fork() to create a child process
int main () {
    pid_t child_pid;

    // Step 1: Print the process ID of the main program
    // (Hint: Use getpid() to obtain the process ID)
	printf("The main process PID is %d.\n", (int) getpid());

    // Step 2: Call fork() to create a child process and store the returned pid
	int pid = fork();
    // Step 3: Check if the process is the parent or the child
    if (pid != 0) {
        // Step 4: If parent, print the parent's PID and the child's PID
		printf("The parent's PID is %d.\n", (int) getpid());
		printf("The child PID is %d.\n", pid);
    } else {
        // Step 5: If child, print the child's PID
		printf("This is the child process. The PID is %d.\n", (int) getpid());
    }

    // Step 6: Return 0 to indicate successful execution
    return 0;
}
