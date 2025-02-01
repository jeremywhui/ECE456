#include <stdio.h>

// read from file.txt

int main () {
   FILE *fp;
   int c;

   fp = fopen("file.txt","r");
   while(1) {
      c = fgetc(fp);
      if( feof(fp) ) {
         break ;
      }
      // print out each char read from file.txt
      printf("%c", c);
   }
   fclose(fp);
   return(0);
}
