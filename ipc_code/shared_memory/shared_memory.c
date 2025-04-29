#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
    /* Allocate a shared memory segment. */
    int shared_memory_id = shmget(IPC_PRIVATE,sizeof(int),IPC_CREAT | SHM_R | SHM_W);
    /* Attach the shared memory segment. */
    int *shared_memory = (int *)shmat(shared_memory_id, 0, 0);

    *shared_memory = 0;
    printf("Parent: number in shared memory is %d\n", *shared_memory);

    int pid = fork();
    //Checks if process runs
    if(pid < 0)
    {
        perror("Process creation failed");
    }
    else if(pid != 0) //fork returns non-zero pid of child to parent process
    {
        printf("Parent: number in shared memory is %d\n", *shared_memory);
        *shared_memory = 10;
        printf("Parent: change the number in shared memory to %d\n",*shared_memory);
    }
    else
    {
        printf("Child: number in shared memory is %d\n", *shared_memory);
        for (int i=0; i < 50; i++)
        {
            *shared_memory = *shared_memory+ 1;
        }
        printf("Child: change the number in shared memory to %d\n",*shared_memory);
        /* Detach the shared memory segment. */
        shmdt(shared_memory);
        exit(0); //exit
    }
    printf("Parent: waiting for the child\n");
    wait(NULL); //wait for child
    printf("Parent: number in shared memory is %d\n", *shared_memory);
    /* Detach the shared memory segment. */
    shmdt(shared_memory);
    /* Deallocate the shared memory segment. */
    shmctl(shared_memory_id, IPC_RMID, 0);
    return 0;
}
