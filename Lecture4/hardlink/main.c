#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <errno.h>
#include <string.h>
//Create a hard link and then unlink

int main(void)
{
   char c;
   FILE *fp;
   if (link("dir/data.txt", "myhardlink") == 0)
      printf("Hard link succeeds\n");
   fp = fopen("myhardlink","r");
   printf("File content:");
   c = fgetc(fp);
   while(c!=EOF)
   {
      printf ("%c",c);
      c = fgetc(fp);
   }
   fclose(fp);
   printf("\n");
   sleep(3);
   if (unlink("myhardlink") < 0)
      printf("unlink error");
   printf("file unlinked\n");
   return 0;
}

