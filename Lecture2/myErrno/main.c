#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
int main () {
   FILE *fp;
   /* now let's try to open same file, which does not exist in this directory. */

   // try two ways to print out error string.
   if ((fp = fopen("file.txt", "r")) == NULL) {
      printf("Error: %s\n", strerror(errno));
      printf("--------\n");
      perror("Invalid file");

      exit(EXIT_FAILURE);

   }
   fclose(fp);
   return 0;
}
