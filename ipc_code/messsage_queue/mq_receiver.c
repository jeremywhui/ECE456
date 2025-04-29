#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QUEUE_NAME "/my_mq"
#define MAX_SIZE 8192
  
int main()
{
    mqd_t mq;
    char buffer[MAX_SIZE] = {0};
    struct mq_attr attr;

    // Open the existing message queue
    mq = mq_open(QUEUE_NAME, O_RDONLY);
    if (mq == (mqd_t) - 1) {
        perror("mq_open failed");
        exit(1);
    }
    // Get queue attributes
    if (mq_getattr(mq, &attr) == -1) {
        perror("mq_getattr failed");
        exit(1);
    }
    printf("Waiting for message...\n");

    // Read a message from the queue
    ssize_t bytes_read = mq_receive(mq, buffer, MAX_SIZE, NULL);
    if (bytes_read == -1) {
        perror("mq_receive failed");
        exit(1);
    }
    buffer[bytes_read] = '\0';  // NULL-terminate received message
    printf("Received: %s\n", buffer);

    // Close the message queue
    mq_close(mq);

    // Remove the message queue (cleanup)
    mq_unlink(QUEUE_NAME);
    return 0;
}
