#include <stdio.h> 
#include <stdlib.h> 
// The ENVIRON variable contains the environment.
extern char** environ;

// optional arguments specify environment variable names
// If no argument, print all variables using NVIRON
int main (int argc, char *argv[])
{ 
    char** var;
    char* value;
    if (argc > 1) {
        for (int i=1; i<argc; i++) {
            if ((value = getenv(argv[i])) != NULL) {
                printf("%s=%s\n", argv[i], value);
            }
            else {
                printf("%s undefined\n", argv[i]);
            }
        }
        return 0;
    }
    for (var = environ; *var != NULL; ++var) 
        printf("%s\n", *var); 
    return 0; 
} 
