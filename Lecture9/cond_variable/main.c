#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_QUEUE_SIE 10
// message struct
struct msg {
	struct msg* next;
	int data;		// you can modify this
};

// Queeu struct
struct msg* workq = NULL;
pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t qready = PTHREAD_COND_INITIALIZER;

// global flag to control termination
int running = 1;

// function to process a message (consumer)
void* process_msg(void* arg) {
	struct msg*	mp;

	while (1) {
		pthread_mutex_lock(&qlock);
		// wait for a message to be available
		while (running && workq == NULL) {
			pthread_cond_wait(&qready, &qlock);
		}
		// if no_more flag is set and queue is empty, terminate
		if (!running && (workq == NULL)) {
			pthread_mutex_unlock(&qlock);
			pthread_exit(0);
		}
		// process message
		mp = workq;
		workq = mp->next;
		pthread_mutex_unlock(&qlock);
		// simulate processing
		printf("Processing message with data: %d\n", mp->data);
		sleep(1);
		printf("Done with message data: %d\n", mp->data);
		free(mp);
	}
}

// function to enqueue a new message
void* enqueue_msg(void *arg) {
	int data = (int)(intptr_t) arg;
	printf("Enqueue message data: %d\n", data);
	struct msg *new_msg = malloc(sizeof(struct msg));
	new_msg->data = data;
	new_msg->next = NULL;

	pthread_mutex_lock(&qlock);
	// if queue is empty, the new message becomes the first element
	if (workq == NULL) {
		workq = new_msg;
	} else {
		// otherwise, find the last element and link it to the new message
		struct msg *last = workq;
		while(last->next != NULL) {
			last = last->next;
		}
		last->next = new_msg;
	}
	// signal the consumer that a new message is available
	pthread_cond_signal(&qready);
	pthread_mutex_unlock(&qlock);
}

// function to signal the consumer to stop running
void* stop(void *arg) {
	pthread_mutex_lock(&qlock);
		running = 0;
	pthread_mutex_unlock(&qlock);
	pthread_cond_signal(&qready);
}

// This program uses condition variable to sync consumer and producers
int main() {
	pthread_t	producer[4], consumer, tid;
	// create consumer to process message
	pthread_create(&consumer, NULL, &process_msg, NULL);
	// create producers
	pthread_create(&producer[0], NULL, &enqueue_msg, (void *) 2);
	pthread_create(&producer[1], NULL, &enqueue_msg, (void *) 0);
	sleep(5);
	pthread_create(&producer[2], NULL, &enqueue_msg, (void *) 1);
	pthread_create(&producer[3], NULL, &enqueue_msg, (void *) 8);
	// wait for threads to finish
	for (int i=0; i<4; i++) {
		pthread_join(producer[i], NULL);
	}
	// set termination flag
	pthread_create(&tid, NULL, &stop, NULL);
	pthread_join(tid, NULL);
	pthread_join(consumer, NULL);
  	return 0;
}
