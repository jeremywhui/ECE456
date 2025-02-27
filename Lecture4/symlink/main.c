#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <errno.h>
#include <string.h>
//Create a link to the dir/data.txt and read it through readlink()

int main(void)
{
  char buf[1024];
  char content[1024];
  ssize_t len;
  if(symlink("dir/data.txt", "mylink") != 0)
    printf("Error: %s\n", strerror(errno));
  if ((len = readlink("mylink", buf, sizeof(buf)-1)) != -1)
    buf[len] = '\0';

  printf("The file path: %s\n", buf);
  FILE * pFile; 
  pFile = fopen ("mylink", "r"); 
  fread(content, 10, 1, pFile);
  content[10] = '\0';
  printf("%s\n", content);
  fclose(pFile);

  return 0;
}
