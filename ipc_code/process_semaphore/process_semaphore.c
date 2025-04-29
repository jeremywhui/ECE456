#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define SHM_SIZE 1024
// define union structure
union semun {
    int val;
    struct semid_ds *buf;
    ushort *array;
};

int sem_wait(int semid) {
    struct sembuf semops[1];
    // Wait for semaphore to become available
    semops[0].sem_num = 0;
    semops[0].sem_op = -1;
    semops[0].sem_flg = SEM_UNDO;
    return semop(semid, semops, 1); 
}

int sem_post(int semid) {
    struct sembuf semops[1];
    // Release semaphore
    semops[0].sem_num = 0;
    semops[0].sem_op = 1;
    semops[0].sem_flg = SEM_UNDO;
    return semop(semid, semops, 1); 
}
// This program uses process semaphore to synchronize processes
int main(void) {
    int shmid;
    char *shmaddr;
    int semid;
    union semun semval;
    int i;
    pid_t pid;

    // Create shared memory segment
    shmid = shmget(IPC_PRIVATE, SHM_SIZE, IPC_CREAT | IPC_EXCL | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // Attach shared memory segment to parent process
    shmaddr = shmat(shmid, NULL, 0);
    if (shmaddr == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    // initialize shared memory
    for (i=0; i<SHM_SIZE; i++) {
        shmaddr[i] = 0;
    }

    // Create semaphore
    semid = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0666);
    if (semid == -1) {
        perror("semget");
        exit(1);
    }

    // Initialize semaphore to 1
    semval.val = 1;
    if (semctl(semid, 0, SETVAL, semval) == -1) {
        perror("semctl");
        exit(1);
    }

    // Fork child processes
    for (i=0; i<2; i++) {
        if ((pid = fork()) < 0) {
            perror("fork");
            exit(1);
        }
        else if (pid == 0) {
            // Child process
            printf("Child process %d starts\n", i+1);
            if (sem_wait(semid) == -1) {
                perror("semop");
                exit(1);
            }
            // Write to shared memory
            sprintf(shmaddr + strlen(shmaddr), "Child process %d wrote to shared memory\n", i+1);
            // Release semaphore
            if (sem_post(semid) == -1) {
                perror("semop");
                exit(1);
            }
            exit(0);
        }
    }

    // parent wait
    for (int i=0; i<2; i++) {
        int status;
        wait(&status);
    }
    printf ("parent read:\n%s", shmaddr);

    // release semaphore and shared memory
    semctl(semid, 1, IPC_RMID, semval);
    shmdt(shmaddr);
    return 0;
}

