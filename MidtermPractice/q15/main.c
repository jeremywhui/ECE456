#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int counter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void incrementCounter(int n) {
    for (int i = 0; i < n; i++) {
        pthread_mutex_lock(&mutex);
        counter += 1;
        pthread_mutex_unlock(&mutex);
    }
}

int main() {
    pthread_t threads[5];
    for (int i = 0; i < 5; i++) {
        if (pthread_create(&threads[i], NULL, (void *)incrementCounter, (void *)(size_t)(100 + i)) != 0) {
            printf("Error creating thread\n");
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i],NULL);
    }
    printf("Final value after all updates: %d\n", counter);
}