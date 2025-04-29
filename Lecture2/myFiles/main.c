#include <stdio.h>  // Include standard input/output library
#include <stdlib.h>  // Include standard library for memory allocation and process control
#include <string.h>  // Include string handling functions (for buffer manipulation, etc.)

int main(int argc, char* const argv[]) {
    // Open the file "file.txt" in read/write mode and check if it's opened successfully
    FILE* fp = fopen("file.txt", "w+");
    if (fp == NULL) {
        perror("File error");
        exit(EXIT_FAILURE);
    }
    // Define a string with a message you want to write to the file
    char c[] = "This is a very cool message!"; 
    // Define a buffer to store the data read from the file
    char buffer[100];
    // Write the defined string to the file using fwrite
    fwrite(c, sizeof(c), 1, fp);
    // Rewind the file pointer to the beginning of the file
    rewind(fp);
    // Read the content from the file into the buffer using fread
    fread(buffer, sizeof(c), 1, fp);
    // Print the content of the buffer to the console
    printf("%s\n", buffer);
    // Close the file after operations are completed
    fclose(fp);
    // Return 0 to indicate successful execution
    return 0;
}
