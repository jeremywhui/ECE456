#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define QUEUE_NAME "/my_mq"

// POSIX message queue sender
int main(int argc, char *argv[])
{
    mqd_t mq = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, 0644, NULL);
    char msg[] = "Hello from POSIX MQ";
    int pause = 0;
    if (argc > 1) {
        pause = atoi(argv[1]);
    }
    sleep(pause);       // demo receiver waiting
    mq_send(mq, msg, strlen(msg)+1, 0);
    mq_close(mq);
    // if want to delete, mq_unlink(QUEUE_NAME);    
    return 0;
}
