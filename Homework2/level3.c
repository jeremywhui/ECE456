#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 8

// Global totalSum, number of items processed, and mutex
int totalSum = 0, processed = 0;
pthread_mutex_t totalSum_mutex = PTHREAD_MUTEX_INITIALIZER;

// Circular buffer
int buffer[BUFFER_SIZE] = {0};
int head = 0;
int tail = 0;

// Two semaphores for available items and remaining space in buffer
sem_t availableItems;
sem_t remainingSpace;

// Producer executes this function, places numbers in buffer
void *addToQueue(void *args) {
    int n = *((int *) args);
    // Sleep for random time between 0 and 1 seconds
    usleep((double) random() / RAND_MAX);
    // Add all items to the queue, while there is space
    for (int i = 1; i <= n; i++) {
        // Wait until there is space
        if (sem_wait(&remainingSpace) == -1) {
            perror("remainingSapce sem_wait");
            exit(EXIT_FAILURE);
        }
        // Update pointers according to circular buffer
        buffer[tail] = i;
        tail = (tail + 1) % BUFFER_SIZE;
        // Add one to the available items
        if (sem_post(&availableItems) == -1) {
            perror("availableItems sem_post");
            exit(EXIT_FAILURE);
        }
    }
    return NULL;
}

// Consumer executes this function, reads from the buffer and updates totalSum 
void *computeSquares(void * args) {
    while (1) {
        if (sem_wait(&availableItems) == -1) {
            perror("availableItems sem_wait");
            exit(EXIT_FAILURE);
        }
        // Mutex to protect access to the buffer, totalSum variable, and processed variable
        pthread_mutex_lock(&totalSum_mutex);
        // Update pointers according to the circular buffer
        int val = buffer[head];
        head = (head + 1) % BUFFER_SIZE;
        // Add the value squared and increase the number processed
        totalSum += val * val;
        printf("Thread %lu incremented total sum by %d^2 = %d. Total Sum = %d.\n", pthread_self(), val, val * val, totalSum);
        processed++;
        pthread_mutex_unlock(&totalSum_mutex);
        if (sem_post(&remainingSpace) == -1) {
            perror("remainingSpace sem_post");
            exit(EXIT_FAILURE);
        }
    }
    return NULL;
}

int main(int argc, char const *argv[]) {
    srandom(time(NULL));
    // Check that there are 3 arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: ./main n numThreads\n");
        exit(EXIT_FAILURE);
    }
    // Set the first parameter to n and numThreads to the second parameter
    int n = atoi(argv[1]), numThreads = atoi(argv[2]);
    // Check that n is in range
    if (n < 10 || n > 1000) {
        fprintf(stderr, "n is not in range (must be between 10 and 1000 inclusive)\n");
        exit(EXIT_FAILURE);
    }
    // Check that numThreads is in range
    if (numThreads < 2 || numThreads > n/2 || numThreads > 100) {
        fprintf(stderr, "numThreads is not in range (must be between 2 and n/2 inclusive, with a maximum of 100)\n");
        exit(EXIT_FAILURE);
    }
    // Initialize the available items semaphore
    if (sem_init(&availableItems, 0, 0) < 0) {
        perror("availableItems sem_init");
        exit(EXIT_FAILURE);
    }
    // Initialize the remaining space semaphore
    if (sem_init(&remainingSpace, 0, BUFFER_SIZE)) {
        perror("remainingSpace sem_init");
        exit(EXIT_FAILURE);
    }
    // Create a thread for the producer
    pthread_t producerThread;
    pthread_create(&producerThread, NULL, &addToQueue, &n);
    // Create threads for the consumers
    pthread_t consumerTid[numThreads];
    for (int i = 0; i < numThreads; i++) {
        if (pthread_create(&consumerTid[i], NULL, &computeSquares, NULL) < 0) {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }
        printf("Created thread %lu\n", consumerTid[i]);
    }
    // Wait for the producer to finish
    pthread_join(producerThread, NULL);
    printf("Finished adding numbers\n");
    // Wait until all items have been processed
    while (processed < n);
    // Threads only terminate once all numbers have been processed
    for (int i = 0; i < numThreads; i++) {
        if (pthread_detach(consumerTid[i]) < 0) {
            perror("Thread detach failed");
            exit(EXIT_FAILURE);
        }
        printf("Thread %lu terminated\n", consumerTid[i]);
    }
    // Report back the final total sum
    printf("All threads have terminated, the final sum is %d.\n", totalSum);
    return 0;
}
