#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define NLOOP 500
/*
Two threads: each thread will increase the global counter NLOOP times, and print it out with the thread ID.
*/
int counter;    /* this is incremented by the threads */
//Initialization
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

void *doit(void *vptr) {
  printf("Entering the thread %lu\n", pthread_self());
  int   i, val;
  for (i = 0; i < NLOOP; i++) {
    pthread_mutex_lock(&counter_mutex);
    val = counter;
    printf("%lu: %d\n", pthread_self(), val + 1);
    counter = val + 1;
    pthread_mutex_unlock(&counter_mutex);
  }
  return(NULL);
}

int main(int argc, char **argv) {
  pthread_t tidA, tidB;
  pthread_create(&tidA, NULL, &doit, NULL);
  printf("Created thread %lu\n", tidA);
  pthread_create(&tidB, NULL, &doit, NULL);
  printf("Created thread %lu\n", tidB);
  /*wait for both threads to terminate */
  pthread_join(tidA, NULL);
  printf("Thread %lu is terminated.\n", tidA);
  pthread_join(tidB, NULL);
  printf("Thread %lu is terminated.\n", tidB);
  exit(0);
}



