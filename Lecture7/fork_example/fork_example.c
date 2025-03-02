#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main() {
  pid_t pid;
  printf("the main program process ID is %d\n", (int)getpid());
  pid = fork();
  if (pid != 0) {
    // wait(NULL);
    sleep(1);
    printf("this is the parent process, with id %d\n", (int)getpid());
    printf("the child's process ID is %d\n", (int)pid);
  } else {
    printf("This value should be 0: %d\n", pid);
    printf("this is the child process, with id %d\n", (int)getpid());
    printf("My parent pid is %d\n", (int)getppid());
  }
  return 0;
}
