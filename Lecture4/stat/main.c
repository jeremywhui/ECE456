#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


//Querying and manipulating a file’s st_mode bits, and file size.

int main (int argc, char **argv){
  struct stat st;
  char *type, *readok;
  stat(argv[1], &st);
  if (S_ISREG(st.st_mode))  /* Determine file type */
    type = "regular";
  else if (S_ISDIR(st.st_mode))
    type = "directory";
  else
    type = "other";
  if ((st.st_mode & S_IRUSR)) /* Check read access */
    readok = "yes";
  else
    readok = "no";
  printf("type: %s, read: %s\n", type, readok);
  printf("File size: %jd\n", st.st_size);
  return 0; 
}
