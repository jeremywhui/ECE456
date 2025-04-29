/* fseek example */ 
#include <stdio.h> 
// This program modifies a specific part of a file using fseek()

int main () { 
  // Open a file pointer
  FILE *fp;
  // Open "example.txt" in write mode
  fp = open("example.txt", "w");
  // Write "This is an apple." to the file pointer 
  fputs("This is an apple.", fp);
  // Set file position indicator to be 9 from the front
  rewind(fp);
  // Write " sam" to the current position
  fputs(" sam", fp);
  // Close the stream
  fclose(fp);
  // Return 0
  return 0;
 }
