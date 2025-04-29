#include <stdio.h>
#include <string.h>
#include <errno.h>
int main () {
   FILE *fp;

   /* now let's try to open same file, which does not exist in this directory. */

   // try two ways to print out error string.
   fp = fopen("file.txt", "r");
   if( fp == NULL ) {
   	  printf("Error: %s\n", strerror(errno));
   	  printf("----------\n");
      
      perror("Error");
      return(-1);
   }
   fclose(fp);
      
   return(0);
}
