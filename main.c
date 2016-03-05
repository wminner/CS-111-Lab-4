#include <stdlib.h> // exit
#include <stdio.h>  // printf

#include <unistd.h> // getopt_long
#include <getopt.h> // struct option (longopts)

int main(int argc, char **argv)
{
    int exit_status = 0;    // Keeps track of how the program should exit

    int next_option;
    int index;
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

    if (argc <= 1) {		// No arguements
        printf("usage: %s [--threads=#] [--iterations=#] [--iter=#] [--yield=#] [--sync=[msc]]\n", argv[0]);
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
                    
                    printf("Found threads = %d.\n", atoi(optarg));

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
                    
                    printf("Found iterations = %d.\n", atoi(optarg));

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
                    
                    printf("Found yield = %d.\n", atoi(optarg));

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
                    
                    printf("Found sync = %d.\n", atoi(optarg));

                    args_found = 0;     // Reset args found for next option
                    break;

                // Default
                default:
                    fprintf (stderr, "Error: unrecognized option \"%s\"\n", argv[optind-1]);
                    exit_status = 1;
            }
        }
    }

    exit(exit_status);
}