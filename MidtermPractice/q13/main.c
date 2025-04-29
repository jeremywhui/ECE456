#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char * const argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: ./main a b\n");
        exit(EXIT_FAILURE);
    }
    int a = atoi(argv[1]), b = atoi(argv[2]);
    int a2, b2;
    int fd[2];
    if (a < -100 || a > 100) {
        fprintf(stderr, "a is out of range\n");
        exit(EXIT_FAILURE);
    }
    if (b < -100 || b > 100) {
        fprintf(stderr, "b is out of range\n");
        exit(EXIT_FAILURE);
    }
    if (pipe(fd) < 0) {
        fprintf(stderr, "pipe error\n");
        exit(EXIT_FAILURE);
    }
    int pid1 = fork();
    if (pid1 < 0) {
        printf("Fork failure\n");
        exit(EXIT_FAILURE);
    } else if (pid1 == 0) {
        // Write a^2 to the pipe
        int n = a * a;
        write(fd[1], &n, sizeof(int));
        exit(0);
    } else {
        wait(NULL);
    }
    if (read(fd[0], &a2, sizeof(int)) < 0) {
        perror("Read failure\n");
        exit(EXIT_FAILURE);
    }
    int pid2 = fork();
    if (pid2 < 0) {
        printf("Fork failure\n");
        exit(EXIT_FAILURE);
    } else if (pid2 == 0) {
        // Write b^2 to the pipe
        int n = b * b;
        write(fd[1], &n, sizeof(int));
        exit(0);
    } else {
        wait(NULL);
    }
    if (read(fd[0], &b2, sizeof(int)) < 0) {
        perror("Read failure\n");
        exit(EXIT_FAILURE);
    }
    printf("Result = %d\n", (a2 - b2));
    return 0;
}