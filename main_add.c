#define _GNU_SOURCE
#include <stdlib.h>     // exit
#include <stdio.h>      // printf
#include <unistd.h>     // getopt_long
#include <getopt.h>     // struct option (longopts)
#include <pthread.h>    // threads
#include <time.h>       // clock_gettime
#include <errno.h>      // errno
#include <string.h>     // strerror

// Globals
long long counter = 0;
int opt_yield = 0;   // Yield in the middle of add to cause race condition
char opt_sync = '0';  // Synchronization method option

// Prototypes
void *doAdd(void *iterations);                  // Function wrapper for add with no synchronization
void *doAddWithMutex(void *iterations);         // Function wrapper for add with mutex synchronization
void *doAddWithSpinLock(void *iterations);      // Function wrapper for add with spin lock synchronization
void *doAddWithCompareSwap(void *iterations);   // Function wrapper for add with compare and swap synchronization
void add(long long *pointer, long long value);  // Basic add routine
void addWithCompareSwap(long long *pointer, long long value);   // Add routine with compare and swap synchronization

int main(int argc, char **argv)
{
    int num_threads = 1;    // Number of threads, default = 1
    int num_iterations = 1; // Number of iterations, default = 1
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
                    
                    if ( *optarg == '0' || *optarg == '1' )
                        opt_yield = atoi(optarg);
                    else {
                        fprintf(stderr, "Error: yield must be 0 or 1\n");
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
                    
                    if ( *optarg == 'm' || *optarg == 's' || *optarg == 'c' )
                        opt_sync = optarg[0];
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

    int i;
    int retval;
    struct timespec tp_start, tp_end;

    // Start timer to track wall time
    retval = clock_gettime(CLOCK_MONOTONIC, &tp_start);
    if ( retval < 0 ) {
        fprintf(stderr, "Error with clock_gettime: %s\n", strerror(errno));
        exit_status = 1;
        goto errorNoFree;
    }

    // Allocate array for thread ids
    pthread_t *threads = (pthread_t*) calloc(num_threads, sizeof(pthread_t));
    if ( !threads ) {
        fprintf(stderr, "Error: could not allocate memory.\n");
        exit_status = 1;
        goto errorNoFree;
    }

    // Create threads
    for ( i = 0; i < num_threads; i++ ) {
        switch (opt_sync) {
            case '0':   // No synchronization
                retval = pthread_create(&threads[i], NULL, &doAdd, &num_iterations);
                break;
            case 'm':   // Mutex
                retval = pthread_create(&threads[i], NULL, &doAddWithMutex, &num_iterations);
                break;
            case 's':   // Spin-lock
                retval = pthread_create(&threads[i], NULL, &doAddWithSpinLock, &num_iterations);
                break;
            case 'c':   // Compare and swap
                retval = pthread_create(&threads[i], NULL, &doAddWithCompareSwap, &num_iterations);
                break;
            default:    // Error
                fprintf(stderr, "Error: invalid sync value\n");
                exit_status = 1;
                goto error;
        }

        if ( retval != 0 ) {  // Error handling
            fprintf(stderr, "Error: could not create requested number of threads.\n");
            exit_status = 1;
            goto error;
        }
    }

    // Wait for all threads to finish
    for ( i = 0; i < num_threads; i++ ) {
        retval = pthread_join(threads[i], NULL);
        if ( retval != 0 ) {
            fprintf(stderr, "Error: failed to join threads.\n");
            exit_status = 1;
            goto error;
        }
    }

    // Stop timer and calculate wall time
    retval = clock_gettime(CLOCK_MONOTONIC, &tp_end);
    if ( retval < 0 ) {
        fprintf(stderr, "Error with clock_gettime: %s\n", strerror(errno));
        exit_status = 1;
        goto error;
    }

    // Calculate wall time and operations
    long long unsigned total_time = 1000000000*(tp_end.tv_sec - tp_start.tv_sec) + (tp_end.tv_nsec - tp_start.tv_nsec);
    long long unsigned num_ops = num_threads*num_iterations*2;
    long long unsigned time_per_op = total_time/num_ops;

    // Print summary of results
    printf("%d threads x %d iterations x (add + subtract) = %llu operations\n", num_threads, num_iterations, num_ops);
    if ( counter != 0 )
        fprintf(stderr, "ERROR: final count = %lld\n", counter);
    else
        printf("final count = %lld\n", counter);
    printf("elapsed time: %llu ns\n", total_time);
    printf("per operation: %llu ns\n", time_per_op);

    error:
    // Free allocated memory
    free(threads);

    errorNoFree:
    exit(exit_status);
}

// Function wrapper for add with no synchronization
void * doAdd(void *iterations) {
    int i;

    // Perform add operations with requested number of threads and iterations
    for ( i = 0; i < *((int*)iterations); i++ )
        add(&counter, 1);
    for ( i = 0; i < *((int*)iterations); i++ )
        add(&counter, -1);
    return 0;
}

// Function wrapper for add with mutex synchronization
void * doAddWithMutex(void *iterations) {
    int i;
    static pthread_mutex_t mutex;

    // Perform add operations with requested number of threads and iterations
    for ( i = 0; i < *((int*)iterations); i++ ) {
        pthread_mutex_lock(&mutex);
        add(&counter, 1);
        pthread_mutex_unlock(&mutex);
    }
    for ( i = 0; i < *((int*)iterations); i++ ) {
        pthread_mutex_lock(&mutex);
        add(&counter, -1);
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}

// Function wrapper for add with spin lock synchronization
void * doAddWithSpinLock(void *iterations) {
    int i;
    static volatile int spinlock = 0;

    // Perform add operations with requested number of threads and iterations
    for ( i = 0; i < *((int*)iterations); i++ ) {
        while ( __sync_lock_test_and_set(&spinlock, 1) );
        add(&counter, 1);
        __sync_lock_release(&spinlock);
    }
    for ( i = 0; i < *((int*)iterations); i++ ) {
        while ( __sync_lock_test_and_set(&spinlock, 1) );
        add(&counter, -1);
        __sync_lock_release(&spinlock);
    }
    return 0;
}

// Function wrapper for add with compare and swap synchronization
void * doAddWithCompareSwap(void *iterations) {
    int i;

    // Perform add operations with requested number of threads and iterations
    for ( i = 0; i < *((int*)iterations); i++ )
        addWithCompareSwap(&counter, 1);
    for ( i = 0; i < *((int*)iterations); i++ )
        addWithCompareSwap(&counter, -1);
    return 0;
}

// Basic add routine
void add(long long *pointer, long long value) {
    long long sum = *pointer + value;
    if ( opt_yield )
        pthread_yield();
    *pointer = sum;
}

// Add routine with compare and swap synchronization
void addWithCompareSwap(long long *pointer, long long value) {
    long long original, sum;
    do {
        original = *pointer;
        sum = original + value;
        if ( opt_yield )
            pthread_yield();
    } while ( __sync_val_compare_and_swap(pointer, original, sum) != original );
}