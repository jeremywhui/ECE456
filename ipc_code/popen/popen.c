#include <stdio.h>
#include <stdlib.h>

// write 0-num to STREAM
void
write_data (FILE * stream, int num)
{
	int i;
	for (i = 0; i <= num; i++)
		fprintf (stream, "%d\n", i);
  	if (ferror (stream))
    {
      	fprintf (stderr, "Output to stream failed.\n");
      	exit (EXIT_FAILURE);
    }
}

// Open a pipe and direct output to the pipe
int
main (int argc, char *argv[])
{
  	FILE *output;
    int num = 15;
    if (argc > 1) {
        num = atoi(argv[1]);
    }
  	output = popen ("sort", "w");
  	if (!output)
    {
      	fprintf (stderr, "incorrect parameters or too many files.\n");
      	return EXIT_FAILURE;
    }
    printf("sort integers from 0 to %d alphabetically\n", num);
  	write_data (output, num);
  	if (pclose (output) != 0)
    {
      	fprintf (stderr, "Could not run more or other error.\n");
    }
  	return EXIT_SUCCESS;
}
