#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>

const char * program_name; // Used in print_usage

// Displayed when help option is called or program is called incorrectly
void print_usage(FILE *stream, int exit_code) 
{
    fprintf(stream, "Usage: %s [--help | --systemcalls] filename arg1 [...]\n", program_name);
    fprintf(stream, 
        "  -h --help            Display this usage information.\n"
        "  -s --systemcalls     Switch to the system calls implementation.\n");
    exit(exit_code);    
}

// Used to count the number of substring occurences
int countSubstrings(char* buffer, char* substring, int size) {
   char* ptr = buffer;
   int count = 0;
   // Get length of current substring
   int substringLen = strlen(substring);
   for (int j = 0; j < (size - substringLen + 1); j++) {
      if (strncasecmp(ptr++, substring, substringLen) == 0) {
         count++;
      }
   }
   return count;
}

// Read a file and count the number of occurences of each substring.
int main (int argc, char **argv) {

   int next_opt;
   int system_flag = 0;
   // Short option string 
   const char* const short_options = "hs";
   // Long option array
   const struct option long_options[] = {
      { "help", 0, NULL, 'h'},
      { "systemcalls",   0,  NULL, 's' },
      { NULL,     0,  NULL,  0  }     // Required at end of array
   };

   program_name = argv[0];         // Save program name for print_usage

   while(1) {		// parse options until no more left
      next_opt = getopt_long(argc, argv, short_options, long_options, NULL);
      if (next_opt < 0) {  // no more options
          break;
      }
      switch (next_opt) {
      case 'h':   // -h or --help
         print_usage(stdout, 0); // print to stdout and exit(0)
      case 's': // -s or --systemcalls: System calls implementation
         system_flag = 1;
         break;
      case '?':   // User entered an invalid option
         print_usage(stderr, 1); // Print to stderr and exit(1)
      default:    // Something else unexpected
         abort();
      }
   }

   if (system_flag == 1) {
      // Systemcalls implementation
      // If not enough command line arguments, throw an error
      if (argc - optind < 3) {
         print_usage(stderr, 1);
      }
      int fd = open(argv[optind], O_RDONLY, 0);
      // Error opening file descriptor
      if (fd < 0) {
         perror(program_name);
         exit(1);
      }
      // Use struct stat to find the file size
      struct stat st;
      stat(argv[optind], &st);
      long size = st.st_size;

      // Find the size of the file and initialize read buffer
      char* buffer = (char *) calloc(size, sizeof(char));
      char* ptr = buffer;
      // Check for calloc error
      if (buffer == NULL) {
         perror(program_name);
         exit(1);
      }
      // Read all characters into the buffer
      for (long i = 0; i < size; i++) {
         read(fd, ptr++, 1);
      }
      // Iterate through command line arguments
      for (int i = optind + 1; i < argc; i++) {
         printf("%d\n", countSubstrings(buffer, argv[i], size));
      }
      // Attempt to close the file descriptor
      if (close(fd) != 0) {
         perror(program_name);
         exit(1);
      }
      // Free the buffer
      free(buffer);
   } else {
      // Standard I/O Implementation
      // If not enough command line arguments, throw an error
      if (argc < 3) {
         print_usage(stderr, 1);
      }
      // Find the size of the file and initialize read buffer
      struct stat st;
      stat(argv[1], &st);
      long size = st.st_size;
      char* buffer = (char *) calloc(size, sizeof(char));
      // Check for calloc error
      if (buffer == NULL) {
         perror(program_name);
         exit(1);
      }

      // Open file for both reading and writing
      FILE* fp = fopen(argv[1], "r");
      if (fp == NULL) {
         perror(program_name);
         exit(1);
      }

      // Read in entire file into buffer
      if (fread(buffer, sizeof(char), size, fp) == -1) {
         perror(program_name);
         exit(1);
      }
      // Count the occurences for each substring
      for (int i = 2; i < argc; i++) {
         printf("%d\n", countSubstrings(buffer, argv[i], size));
      }

      /* Close the file pointer */
      if (fclose(fp) != 0) {
         perror(program_name);
         exit(1);
      }

      // Free the buffer
      free(buffer);
   }
   return(0);
}