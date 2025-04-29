#include <stdio.h>  // Include the standard input/output library for file handling and printing

// Open the file "file.txt" for reading and check if it's opened successfully

int main () {
   // Declare a file pointer variable to handle the file
   // Declare a variable to store each character read from the file
   FILE* fp;
   int c;
   // Open "file.txt" in read mode
   fp = fopen("file.txt", "r");
   // Start an infinite loop to read characters from the file
   while (1) {
      // Read a character from the file using fgetc
      c = fgetc(fp);
      // Check if the end of the file is reached (EOF)
      if (feof(fp)) {
         // Exit the loop if the end of the file is encountered
         break;
      }
      // Print out the character read from the file
      printf("%c", c);
   }
   // Close the file after reading
   fclose(fp);
   // Return 0 to indicate successful execution
   return 0;
}
