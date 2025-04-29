#include <sys/types.h>  // Include system types for low-level I/O operations
#include <sys/uio.h>    // Include I/O functions for system calls
#include <unistd.h>     // Include POSIX operating system API
#include <stdio.h>      // Include standard input/output library
#include <stdlib.h>     // Include standard library for memory allocation
#include <fcntl.h>      // Include file control library for opening files

// Implement the main function where the file reading process occurs
int main() {

  // Declare a pointer to hold the buffer that will store the file content
  // Declare variables for the file descriptor and the number of bytes read
  char *c;
  int fd, sz;
  // Allocate memory for the buffer to store the read data
  c = (char *)calloc(100, sizeof(char));
  // Open the file "file.txt" in read-only mode using the system call open
  // If opening the file fails, print an error message and exit the program
  fd = open("file.txt", O_RDONLY, 0);
  if (fd < 0) {perror("r1"); exit(EXIT_FAILURE);}
  // Read the first 20 bytes of the file using the read system call
  sz = read(fd, c, 20);
  // Print the number of bytes that were successfully read
  printf("Returned that %d bytes were read.\n", sz);
  // Add a null-terminator to the buffer to ensure it's properly formatted as a string
  c[sz] = '\0';
  // Print the content of the buffer that contains the bytes read from the file
  printf("%s\n", c);
  return 0;
}
