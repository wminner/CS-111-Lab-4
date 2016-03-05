#include <stdlib.h>     // exit
#include <stdio.h>      // printf

#include <unistd.h>     // getopt_long
#include <getopt.h>     // struct option (longopts)

#include <pthread.h>    // threads

// Globals
long long counter = 0;

// Prototypes
void *doAdd(void *iterations);                   // Function wrapper for add to pass into pthread_create
void add(long long *pointer, long long value);   // Basic add routine

int main(int argc, char **argv)
{
    int num_threads = 1;    // Number of threads, default = 1
    int num_iterations = 1; // Number of iterations, default = 1
    char opt_yield = '0';   // Yield in the middle of add to cause race condition
    char opt_sync = '0';    // Synchronization method option
    int exit_status = 0;    // Keeps track of how the program should exit

    int next_option;        // Return value of getopt_long
    int index;              // Index into optarg
    int option_index = 0;   // Used with getopt_long
    int currOptInd = 0;     // Current option index
    int args_found = 0;     // Used to verify --option num of arguments requirement

    extern char *optarg;    // Gives the option strings
    extern int optind;      // Gives the current option out of argc options
    extern int opterr;      // Declared in getopt_long
    opterr = 0;             // Turns off automatic error message from getopt_long


	static struct option long_options[] =
    {
    	{"threads",    required_argument, 0, 't' },
        {"iterations", required_argument, 0, 'i' },
        {"iter",       required_argument, 0, 'j' },
        {"yield",      required_argument, 0, 'y' },
        {"sync",       required_argument, 0, 's' },
        {0, 0, 0, 0}
    };

    // BEGIN parsing options
    if (argc <= 1) {		// No arguements
        printf("usage: %s [--threads=#] [--iterations=#] [--iter=#] [--yield=#] [--sync=[msc]]\n", argv[0]);
        exit(0);
    } else if (argc > 1) { 	// At least one argument
        while (1) {
            // Get next option
            next_option = getopt_long(argc, argv, "", long_options, &option_index);
            currOptInd = optind;
            index = 0;
            
            // No more options found, then break out of while loop
            if (next_option == -1)
                break;

            switch (next_option) {
                // Occurs for options that set flags
                case 0: 

                    break;

                // Threads
                case 't':
                    while (currOptInd <= argc) {
                        if (optarg[index] == '-' && optarg[index+1] == '-') // Check for '--'
                            break;
                        else    // Else, found another argument
                            args_found++;
                        while (optarg[index] != '\0')
                            index++;
                        index++;
                        currOptInd++;
                    }

                    if (args_found != 1)    // Error if num of args not 1
                    {
                        fprintf (stderr, "Error: \"--threads\" requires one argument.  You supplied %d arguments.\n", args_found);
                        exit_status = 1;
                        if (args_found == 0)    // If no args found, decrement optind so we don't skip the next option
                            optind--;
                        args_found = 0;
                        break;
                    }
                    
                    //printf("Found threads = %d.\n", atoi(optarg));
                    if ( atoi(optarg) >= 1 )
                        num_threads = atoi(optarg);
                    else {
                        fprintf(stderr, "Error: number of threads must be greater than or equal to 1!\n");
                        exit_status = 1;
                    }

                    args_found = 0;     // Reset args found for next option
                    break;

                // Iterations
                case 'i':
                case 'j':
                    while (currOptInd <= argc) {
                        if (optarg[index] == '-' && optarg[index+1] == '-') // Check for '--'
                            break;
                        else    // Else, found another argument
                            args_found++;
                        while (optarg[index] != '\0')
                            index++;
                        index++;
                        currOptInd++;
                    }

                    if (args_found != 1)    // Error if num of args not 1
                    {
                        fprintf (stderr, "Error: \"--iterations\" or \"--iter\" requires one argument.  You supplied %d arguments.\n", args_found);
                        exit_status = 1;
                        if (args_found == 0)    // If no args found, decrement optind so we don't skip the next option
                            optind--;
                        args_found = 0;
                        break;
                    }
                    
                    //printf("Found iterations = %d.\n", atoi(optarg));
                    if ( atoi(optarg) >= 1 )
                        num_iterations = atoi(optarg);
                    else {
                        fprintf(stderr, "Error: number of iterations must be greater than or equal to 1!\n");
                        exit_status = 1;
                    }

                    args_found = 0;     // Reset args found for next option
                    break;

                // Yield
                case 'y':
                    while (currOptInd <= argc) {
                        if (optarg[index] == '-' && optarg[index+1] == '-') // Check for '--'
                            break;
                        else    // Else, found another argument
                            args_found++;
                        while (optarg[index] != '\0')
                            index++;
                        index++;
                        currOptInd++;
                    }

                    if (args_found != 1)    // Error if num of args not 1
                    {
                        fprintf (stderr, "Error: \"--yield\" requires one argument.  You supplied %d arguments.\n", args_found);
                        exit_status = 1;
                        if (args_found == 0)    // If no args found, decrement optind so we don't skip the next option
                            optind--;
                        args_found = 0;
                        break;
                    }
                    
                    //printf("Found yield = %c.\n", *optarg);
                    if ( *optarg == '0' || *optarg == '1' )
                        opt_yield = *optarg;
                    else {
                        fprintf(stderr, "Error: yield must be from [01ids]!\n");
                        exit_status = 1;
                    }

                    args_found = 0;     // Reset args found for next option
                    break;

                // Sync
                case 's':
                    while (currOptInd <= argc) {
                        if (optarg[index] == '-' && optarg[index+1] == '-') // Check for '--'
                            break;
                        else    // Else, found another argument
                            args_found++;
                        while (optarg[index] != '\0')
                            index++;
                        index++;
                        currOptInd++;
                    }

                    if (args_found != 1)    // Error if num of args not 1
                    {
                        fprintf (stderr, "Error: \"--sync\" requires one argument.  You supplied %d arguments.\n", args_found);
                        exit_status = 1;
                        if (args_found == 0)    // If no args found, decrement optind so we don't skip the next option
                            optind--;
                        args_found = 0;
                        break;
                    }
                    
                    //printf("Found sync = %c.\n", *optarg);
                    if ( *optarg == 'm' || *optarg == 's' || *optarg == 'c' )
                        opt_sync = *optarg;
                    else {
                        fprintf(stderr, "Error: sync must be from [msc]!\n");
                        exit_status = 1;
                    }

                    args_found = 0;     // Reset args found for next option
                    break;

                // Default
                default:
                    fprintf (stderr, "Error: unrecognized option \"%s\"\n", argv[optind-1]);
                    exit_status = 1;
            }
        }
    }
    // END parsing options

    // Allocate array for thread ids
    pthread_t *threads = (pthread_t*) calloc(num_threads, sizeof(pthread_t));

    int i;
    // Create threads
    for ( i = 0; i < num_threads; i++ ) {
        int retval = pthread_create(&threads[i], NULL, &doAdd, &num_iterations);
        if (retval != 0) {  // Error handling
            fprintf(stderr, "Error: could not create requested number of threads.\n");
            exit_status = 1;
        }
    }

    // Print result
    printf("Result of add is %lld\n", counter);

    // Free allocated memory
    free(threads);

    exit(exit_status);
}

// Function wrapper for add to pass into pthread_create
void * doAdd(void *iterations) {
    int i;
    // Perform add operations with requested number of threads and iterations
    for ( i = 0; i < *((int*)iterations); i++ )
        add(&counter, 1);
    for ( i = 0; i < *((int*)iterations); i++ )
        add(&counter, -1);
    return 0;
}

// Basic add routine
void add(long long *pointer, long long value) {
    long long sum = *pointer + value;
    *pointer = sum;
}