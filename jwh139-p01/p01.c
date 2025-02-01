#include <stdio.h>
#include <string.h>
//write a string to a file, and read the file and display the string.
int main () {
   FILE *fp;
   char c[] = "Network Centric Programming";
   char buffer[100];

   /* Open file for both reading and writing */
   fp = fopen("file.txt", "w+");

   /* Write data to the file */
   fwrite(c, sizeof(c), 1, fp);

   /* Seek to the beginning of the file */
   fseek(fp, 0, SEEK_SET);

   /* Read and display data */
   fread(buffer, sizeof(c), 1, fp);
   printf("%s\n", buffer);
   fclose(fp);
   
   return(0);
}