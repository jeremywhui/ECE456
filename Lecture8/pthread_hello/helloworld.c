#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *function_hello(void *vargp) /* Thread routine */
{
	printf("Hello, world!\n");
	printf("Thread id = %ld \n", pthread_self());
	return NULL;
}

int main(){
	pthread_t tid;
	pthread_create(&tid, NULL, function_hello, NULL);
	printf("Waiting for the thread %ld to terminate.\n", tid);
	pthread_join(tid, NULL);
	exit(0);
}
