#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>

// function to print a single character of the string at a time
static void charatatime(char *str) ;
void err_sys(const char* x);

// This program uses named semaphore to prevent race condition
int main(void) {
    pid_t pid;
    sem_t *semp;
    char *sem_name = "/my_semaphore";

    // Create/open a semaphore
    semp = sem_open (sem_name, O_CREAT, S_IRUSR | S_IWUSR, 1);
    if (semp == NULL) {
        perror("sem_open");
        exit(1);
    }
    // Fork child process
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }
    else if (pid == 0) {
        // Child process - wait for semaphore
        if (sem_wait(semp) == -1) {
            perror("sem_wait");
            exit(1);
        }
        // critical section
        charatatime("Child process wants to print a long string to stdout.\n");
        // Release semaphore
        if (sem_post(semp) == -1) {
            perror("sem_post");
            exit(1);
        }
        sem_close(semp);
        exit(0);
    }
    else {
        // Parent process - wait for semaphore
        if (sem_wait(semp) == -1) {
            perror("sem_wait");
            exit(1);
        }
        charatatime("Parent process is doing something else. The processes are running concurrently.\n");
        // Release semaphore
        if (sem_post(semp) == -1) {
            perror("sem_post");
            exit(1);
        }
        sem_close(semp);
    }
    // Parent wait
    int status;
    wait(&status);

    // Destroy a semaphore
    sem_unlink(sem_name);
    return 0;
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
