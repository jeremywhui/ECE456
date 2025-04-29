#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Helper function to compute the partial sums
int computePartialSum(int from, int to) {
  int pSum = 0;
  for (int i = from; i <= to; i++) {
    pSum += i;
  }
  return pSum;
}

/* Program to compute all numbers from 1 to a specified 
number using a specified number of processes */
int main(int argc, char *const argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: ./main n numProcesses\n");
    exit(EXIT_FAILURE);
  }
  int n = atoi(argv[1]), numProcesses = atoi(argv[2]);
  if (n < 10 || n > 1000) {
    fprintf(stderr, "n is not in range (must be between 10 and 1000 inclusive)\n");
    exit(EXIT_FAILURE);
  }
  if (numProcesses < 2 || numProcesses > n/2 || numProcesses > 100) {
    fprintf(stderr, "numProcesses is not in range (must be between 2 and n/2 inclusive, with a maximum of 100)\n");
    exit(EXIT_FAILURE);
  }
  int pipefd[2];
  if (pipe(pipefd) < 0) {
    perror("Pipe error");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < numProcesses; i++) {
    // Fork numProcess children
    pid_t pid = fork();
    if (pid < 0) {
      // Fork error
      perror("Fork error");
      exit(EXIT_FAILURE);
    } else if (pid == 0) {
      // Child process, close the read end
      close(pipefd[0]);
      int pSum = computePartialSum(i * n / numProcesses + 1, (i + 1) * n/ numProcesses);
      printf("Child with PID %d has computed a partial sum of %d.\n", getpid(), pSum);
      // Write to the pipe and close the write end
      write(pipefd[1], &pSum, sizeof(int));
      close(pipefd[1]);
      exit(0);
    }
  }
  // Wait for all children to terminate
  while (wait(NULL) > 0);
  // Close the write end
  close(pipefd[1]);
  // Calculate the sum
  int sum = 0;
  for (int i = 0; i < numProcesses; i++) {
    int curr;
    read(pipefd[0], &curr, sizeof(int));
    sum += curr;
  }
  printf("All children have terminated, the final sum is %d.\n", sum);
  // Close the read end
  close(pipefd[0]);
  return 0;
}
