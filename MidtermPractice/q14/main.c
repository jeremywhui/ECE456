#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *function_string(char s[]) {
    printf("Thread input: %s\n", s);
    sleep(1);
    return "Call pthread_create()";
}

int main(void) {
    pthread_t id;
    void *returnVal;
    char s[] = "How to create a thread?";
    int ret = pthread_create(&id, NULL, function_string, (void *)s);
    if (ret != 0) {
        printf("Create pthread error!\n");
        exit(1);
    }
    pthread_join(id, &returnVal);
    printf("Peer thread returns: %s\n", (char *) returnVal);
    return 0;
}