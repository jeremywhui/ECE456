#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <semaphore.h>

// function to print a single character of the string at a time
static void charatatime(char *str) ;
void err_sys(const char* x);

// This program uses unnamed semaphore to prevent race condition
int main(void) {
    int shmid;
    char *shmaddr;
    pid_t pid;
    sem_t *semp;

    // Create shared memory segment
    shmid = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | IPC_EXCL | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // Attach shared memory segment to semaphore 
    semp = (sem_t *) shmat(shmid, NULL, 0);
    if (semp < 0) {
        perror("shmat");
        exit(1);
    }
    // Attach shared memory segment to semaphore 
    semp = (sem_t *) shmat(shmid, NULL, 0);
    if (semp < 0) {
        perror("shmat");
        exit(1);
    }

    // POSIX semaphore shared by processes must be in shared memory
    // Initialize semaphore to 1
    if (sem_init(semp, 1, 1) < 0) {
        perror("sem_init");
        exit(1);
    }
    printf("1st sem_init()\n");
    // POSIX semaphore shared by processes must be in shared memory
    // Initialize semaphore to 1
    if (sem_init(semp, 1, 1) < 0) {
        perror("sem_init");
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
    // POSIX semaphore shared by processes must be in shared memory
    // Initialize semaphore to 1
    if (sem_init(semp, 1, 1) < 0) {
        perror("sem_init");
        exit(1);
    }
        printf("child call sem_init()\n");
        if (sem_destroy(semp) < 0) {
            perror("child sem_destroy");
            exit(1);
        }
        if (sem_destroy(semp) < 0) {
            perror("child sem_destroy");
            exit(1);
        }
        printf("child call sem_destroy() twice\n");
        // Child process - wait for semaphore
        if (sem_wait(semp) == -1) {
            perror("sem_wait");
            exit(1);
        }

        // critical section
        //charatatime("Child process wants to print a long string to stdout.\n");
        // Release semaphore
        if (sem_post(semp) == -1) {
            perror("sem_post");
            exit(1);
        }
        sleep(1);
        exit(0);
    }
    else {
        // parent process - wait for semaphore
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
    }
    // parent wait
    int status;
    wait(&status);

    // release semaphore and shared memory
    sem_destroy(semp);
        // Child process - wait for semaphore
        if (sem_wait(semp) == -1) {
            perror("sem_wait");
            exit(1);
        }

        // critical section
        //charatatime("Child process wants to print a long string to stdout.\n");
        // Release semaphore
        if (sem_post(semp) == -1) {
            perror("sem_post");
            exit(1);
        }
    printf("parent call sem_destroy()\n");
    shmdt(semp);
    shmctl(shmid, IPC_RMID, 0);
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
