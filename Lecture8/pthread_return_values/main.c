#include <pthread.h> 
#include <string.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>

void *function_integer(void *arg) 
{ 
    printf("Entering the function_integer() via thread: %ld.\n", pthread_self());
    printf("function_integer() returns.\n"); 
    return((void *)1); 
} 

void *function_string(void *arg) 
{ 
    printf("Entering the function_string() via thread: %ld.\n", pthread_self());
    printf("function_string() returns.\n"); 
    return((void *)"Hello from Linux"); 
} 

int main() 
{ 
    pthread_t tid1,tid2; 
    void *tret; 
    pthread_create(&tid1,NULL,function_integer,NULL); //create thread 1
    printf("Created thread: %ld\n", tid1);
    pthread_create(&tid2,NULL,function_string,NULL); //create thread 2
    printf("Created thread: %ld\n", tid2);
    pthread_join(tid1,&tret); //waiting for the thread 1 termination
    printf("Main process ID: %d. Thread 1's return value: %td\n",(int)getpid(),(intptr_t)tret); 
    pthread_join(tid2,&tret); //waiting for the thread 1 termination
    printf("Main process ID: %d. Thread 2's return value: %s\n",(int)getpid(),(char *)tret); 
    exit(0); 
} 
