#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
//copy stdin to stdout
int main() {
  char buf[10000];
  int n=0;
  while((n=read(STDIN_FILENO, buf, sizeof(buf))) != 0) {
    write(STDOUT_FILENO, buf, n);
  }
  return 0;
}
