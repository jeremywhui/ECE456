#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    FILE *fp;
    fp = fopen("test_10MB.txt", "w+");
    for (int i = 0; i < 10485760; i++) {
        fprintf(fp, "a");
    }
    fclose(fp);
    return 0;
}
