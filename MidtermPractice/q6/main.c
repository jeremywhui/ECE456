#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char * const argv[]) {
    int n = 0;
    printf("Enter an integer to find the factors of: ");
    scanf("%d", &n);
    if (n <= 0) {
        fprintf(stderr, "Input is not valid\n");
        exit(EXIT_FAILURE);
    }
    if (errno == ERANGE) {
        fprintf(stderr, "Input is outside allowable range\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 1; i <= n; i++) {
        if (n % i == 0)
            printf("%d ", i);
    }
    printf("\n");
    return 0;
}