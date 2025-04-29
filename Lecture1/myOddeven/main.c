//Show a number is odd or even.
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    int n;
    printf("Enter an integer: ");
    scanf("%d", &n);
    if (n % 2 == 0) {
        printf("%d is even.\n", n);
    } else {
        printf("%d is odd.\n", n);
    }
    return 0;
}
