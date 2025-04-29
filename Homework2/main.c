#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

// Global totalSum and mutex
int totalSum = 0;
pthread_mutex_t totalSum_mutex = PTHREAD_MUTEX_INITIALIZER;

// Struct for bounds to sum over in threads
typedef struct {
    int from;
    int to;
} bounds;

// Threads execute this function, compute partial sums 
void * computePartialSum(void * boundsArg) {
    bounds myBounds = *((bounds *) boundsArg);
    // Sleep for random time between 0 and 1 seconds
    usleep((double) random() / RAND_MAX);
    int val;
    for (int i = myBounds.from; i <= myBounds.to; i++) {
        // Mutex to protect access to the totalSum variable
        pthread_mutex_lock(&totalSum_mutex);
        // Add the value squared
        totalSum += i * i;
        printf("Total sum incremented by %d^2 = %d. Total Sum = %d.\n", i, i * i, totalSum);
        pthread_mutex_unlock(&totalSum_mutex);
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
    // Create threads to compute partial sums
    pthread_t tid[numThreads];
    bounds *thisBounds = malloc(sizeof(bounds) * numThreads);
    for (int i = 0; i < numThreads; i++) {
        // Set thisBounds for each thread
        thisBounds[i].from = i * n / numThreads + 1;
        thisBounds[i].to = (i + 1) * n/ numThreads;
        if (pthread_create(&tid[i], NULL, &computePartialSum, &thisBounds[i]) < 0) {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }
        printf("Created thread %lu\n", tid[i]);
    }
    // Wait for all threads to finish
    printf("Finished computing\n");
    for (int i = 0; i < numThreads; i++) {
        if (pthread_join(tid[i], NULL) < 0) {
            perror("Thread join failed");
            exit(EXIT_FAILURE);
        }
        printf("Thread %lu terminated\n", tid[i]);
    }
    // Report back the final total sum
    printf("All threads have terminated, the final sum is %d.\n", totalSum);
    return 0;
}
