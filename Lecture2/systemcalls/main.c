#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
//read from a txt file using system calls, and display the number of bytes were read, and the content.
int main() {

  char *c;
  int fd, sz;

  c = (char *) calloc(100, sizeof(char));

  fd = open("file.txt", O_RDONLY, 0);
  if (fd < 0) { perror("r1"); exit(1); }

  sz = read(fd, c, 20);
  printf("Returned that %d bytes were read.\n", sz);
  c[sz] = '\0';
  printf("Those bytes are as follows: %s\n", c);
}
