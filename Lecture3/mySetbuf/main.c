/* setbuf example */
#include <stdio.h>

int main ()
{
  // Create a buffer with 100 characters and two file pointers
  char buffer[100];
  FILE *pfile1, *pfile2;
  // Open two files in write mode to the file pointers
  pfile1 = fopen("myfile1.txt", "w");
  pfile2 = fopen("myfile2.txt", "w");
  // Set the first file pointer of stream to be fully buffered
  // Write a message to the stream
  // Flush the buffer and create an empty buffer
  setvbuf(pfile1, buffer, _IOFBF, sizeof(buffer));
  fputs("This message is sent to the fully buffered stream.", pfile1);
  fflush(pfile1);
  // Set the second file pointer to be unbuffered and write a message to stream
  setvbuf(pfile2, buffer, _IONBF, 0);
  fputs("This message is sent to the unbuffered stream.", pfile2);
  // Close both file pointers and return 0
  fclose(pfile1);
  fclose(pfile2);
  return 0;
}
