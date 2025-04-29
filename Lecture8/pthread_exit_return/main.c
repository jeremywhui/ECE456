#include <stdio.h> 
#include <pthread.h> 
#include <stdlib.h> 

void function_integer(int s) 
{ 
        printf("Entering function_integer() via thread: %ld.\n", pthread_self()); 
        printf("%d\n",s); 
        pthread_exit("Terminate the current thread inside of the function_integer().");  
        //terminate the current thread
} 

void function_string(char s[]) 
{ 
        printf("Entering function_string() via thread: %ld.\n", pthread_self()); 
        printf("%s\n",s); 
        pthread_exit("Terminate the current thread inside of the function_string().");  
        //terminate the current thread
} 

int main(void) 
{ 
        pthread_t id1,id2; 
        void *a1,*a2; 
        int i,ret1,ret2; 
        int s1=1000; //input arguments
        char s2[]="Hello world!"; //input arguments
        ret1=pthread_create(&id1,NULL,(void *) function_integer,(void *)(size_t)s1); 
        printf("Created thread: %ld\n", id1);
        ret2=pthread_create(&id2,NULL,(void *) function_string,(void *) s2); 
        printf("Created thread: %ld\n", id2);

        if(ret1!=0){ 
                printf ("Create pthread1 error!\n"); 
                exit (1); 
        } 
        printf("Main thread is waiting for the thread %ld to terminate.\n", id1);
        pthread_join(id1,&a1); 

        printf("%s\n",(char*)a1); 

        if(ret2!=0){ 
                printf ("Create pthread2 error!\n"); 
                exit (1); 
        } 
        printf("Main thread is waiting for the thread %ld to terminate.\n", id2);
        pthread_join(id2,&a2); 
        printf("%s\n",(char*)a2); 
        return (0); 
} 
