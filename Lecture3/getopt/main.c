#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

// Parse command line options using getopt_long() and print result

const char * program_name;	// used in print_usage()

// print program usage to stream, and exit with exit_code. Not return.
void print_usage (FILE *stream, int exit_code) 
{
    fprintf(stream, "Usage: %s options [ inputfile ]\n", program_name);
    fprintf(stream, 
        "  -h --help            Display this usage information.\n"
        "  -o --output filename Write output to file.\n"
        "  -v --verbose         Print verbose message.\n");
    exit(exit_code);    
}

int main(int argc, char* argv[])
{
    int next_opt;
    // short option string 
    const char* const short_options = "ho:v";
    // long option array
    const struct option long_options[] = {
        { "help",   0,  NULL, 'h' },
        { "output", 1,  NULL, 'o' },
        { "verbose",0,  NULL, 'v' },
        { NULL,     0,  NULL,  0  }     // required at end of array
    };

    const char* output_filename;    // output file name or NULL for stdout
    int verbose = 0;                // whether to display verbose messages

    program_name = argv[0];         // save program name for print_usage

    while(1) {		// parse options until no more left
        next_opt = getopt_long (argc, argv, short_options,
                        long_options, NULL);
        if (next_opt < 0) {  // no more options
            break;
        }
        switch (next_opt)
        {
        case 'h':   // -h or --help
            print_usage(stdout, 0); // print to stdout and exit(0)
        case 'o':   // -o or --output
            // this option takes an argument - output file name
            output_filename = optarg;
            break;
        case 'v':   // -v or --verbose
            verbose = 1;
            break;
        case '?':   // user entered an invalid option
            print_usage(stderr, 1); // print to stderr and exit(1)
        default:    // something else unexpected
            abort();
        }
    }
     
    // for demo only, print output file name if it is specified.
    if (output_filename != NULL) {
        printf("output file is %s\n", output_filename);
    }
    // done with options. OPTIND points to 1st nonoption argument.
    // For demo, print them if verbose option is specified.
    if (verbose) {
        int i;
        for (i=optind; i<argc; i++) {
            printf("argument: %s\n", argv[i]);
        }
    }
    // done
    return 0; 
}
