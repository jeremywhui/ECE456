#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./main n\n");
        exit(EXIT_FAILURE);
    }
    int n = atoi(argv[1]);
    if (n < 0) {
        fprintf(stderr, "Argument is invalid\n");
        exit(EXIT_FAILURE);
    }
    if (n > 30) {
        fprintf(stderr, "Result cannot fit in a 32-bit integer\n");
        exit(EXIT_FAILURE);
    }
    int res = 1;
    for (int i = 0; i < n; i++) {
        res *= 2;
    }
    printf("2^%d = %d\n", n, res);
    return 0;
}
