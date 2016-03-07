#define _GNU_SOURCE
#include <stdlib.h>     // exit
#include <stdio.h>      // printf
#include <unistd.h>     // getopt_long
#include <getopt.h>     // struct option (longopts)
#include <pthread.h>    // threads
#include <time.h>       // clock_gettime
#include <errno.h>      // errno
#include <string.h>     // strerror
#include "SortedList.h" // doubly linked list for part 2 of lab

// Globals
int opt_yield = 0;      // Yield in the middle of add to cause race condition
char opt_sync = '0';    // Synchronization method option
int opt_lists = 1;
int num_threads = 1;    // Number of threads, default = 1
int num_iterations = 1; // Number of iterations, default = 1
int key_length = 10;    // Length of key for SortedListElement
SortedList_t **list_head;       // Shared list to be inserted into/deleted from
SortedListElement_t *elements;  // Elements to be inserted into/deleted from list_head

// Prototypes
void *doTask(void *iterations);                  // Function wrapper for add with no synchronization
void *doTaskWithMutex(void *iterations);         // Function wrapper for add with mutex synchronization
void *doTaskWithSpinLock(void *iterations);      // Function wrapper for add with spin lock synchronization
void *doTaskWithCompareSwap(void *iterations);   // Function wrapper for add with compare and swap synchronization
int findhash(const char *key);                   // Find hash value for a given key mod number of lists

int main(int argc, char **argv)
{
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
        {"lists",      required_argument, 0, 'l' },
        {0, 0, 0, 0}
    };

    // BEGIN parsing options
    if (argc <= 1) {		// No arguements
        printf("usage: %s [--threads=#] [--iterations=#] [--iter=#] [--yield=[ids]] [--sync=[msc]]\n", argv[0]);
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
                    
                    // OR all yield characters from optarg (can be any combination of [ids])
                    for ( unsigned k = 0; k < strlen(optarg); k++ ) {
                        switch (optarg[k]) {
                            case 'i':   // yield during insert
                                opt_yield |= INSERT_YIELD;
                                break;
                            case 'd':   // yield during delete
                                opt_yield |= DELETE_YIELD;
                                break;
                            case 's':   // yield during search
                                opt_yield |= SEARCH_YIELD;
                                break;
                            default:
                                fprintf(stderr, "Error: yield must be from [ids]!\n");
                                exit_status = 1;
                        }
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

                // Lists
                case 'l':
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
                        fprintf (stderr, "Error: \"--lists\" requires one argument.  You supplied %d arguments.\n", args_found);
                        exit_status = 1;
                        if (args_found == 0)    // If no args found, decrement optind so we don't skip the next option
                            optind--;
                        args_found = 0;
                        break;
                    }

                    if ( atoi(optarg) >= 1 )
                        opt_lists = atoi(optarg);
                    else {
                        fprintf(stderr, "Error: list number must be greater than or equal to 1!\n");
                        exit_status = 1;
                    }

                    args_found = 0;
                    break;

                // Default
                default:
                    fprintf (stderr, "Error: unrecognized option \"%s\"\n", argv[optind-1]);
                    exit_status = 1;
            }
        }
    }
    // END parsing options

    int i, j, k;                            // Counters
    int retval;                             // Track return values for error handling
    int result_length = 0;                  // Used for finding final length at end to output
    struct timespec tp_start, tp_end;       // Timers
    int total_elements = num_threads * num_iterations;  // Total num of elements needing to distribute to lists
    pthread_t *threads = NULL;              // Used to hold thread ids

    // These variables were declared earlier in the global scope
    // Initilize and construct list heads according to user specified opt_lists
    list_head = (SortedList_t**) calloc(opt_lists, sizeof(SortedList_t*));
    for ( k = 0; k < opt_lists; k++ )
        list_head[k] = SortedList_new();
    // Allocate array of all elements that will be distributed to threads
    elements = (SortedListElement_t*) calloc(total_elements, sizeof(SortedListElement_t));
    if ( !elements ) {
        fprintf(stderr, "Error: unable to allocate memory.\n");
        exit_status = 1;
        goto error;
    }

    // Seed rand with time, which will be used to generate keys
    srand((unsigned) time(NULL));

    // Initialize all elements with a random key
    for ( i = 0; i < total_elements; i++ ) {
        // Allocate key memory
        char *new_key = (char*) malloc(key_length*sizeof(char) + 1);
        if ( !new_key ) {
            fprintf(stderr, "Error: unable to allocate memory.\n");
            exit_status = 1;
            goto error;
        }
        // Generate random key (10 chars in [A-Za-z])
        for ( j = 0; j < key_length; j++ ) {
            int mod = rand() % 52;
            if ( mod < 26 )
                new_key[j] = 'a' + mod;
            else
                new_key[j] = 'A' + mod - 26;
        }
        new_key[j+1] = '\0';

        // Point element key to new key
        (elements+i)->key = new_key;
    }

    // Start timer to track wall time
    retval = clock_gettime(CLOCK_MONOTONIC, &tp_start);
    if ( retval < 0 ) {
        fprintf(stderr, "Error with clock_gettime: %s\n", strerror(errno));
        exit_status = 1;
        goto error;
    }

    // Allocate array for thread ids
    threads = (pthread_t*) calloc(num_threads, sizeof(pthread_t));
    if ( !threads ) {
        fprintf(stderr, "Error: unable to allocate memory.\n");
        exit_status = 1;
        goto error;
    }

    // Create threads
    switch (opt_sync) {
        case '0':   // No synchronization
            for ( i = 0; i < num_threads; i++ ) {
                int start_pos = num_iterations*i;
                retval = pthread_create(&threads[i], NULL, &doTask, &start_pos);
                if ( retval != 0 ) {  // Error handling
                    fprintf(stderr, "Error: could not create requested number of threads.\n");
                    exit_status = 1;
                    goto error;
                }
            }
            break;
        case 'm':   // Mutex
            for ( i = 0; i < num_threads; i++ ) {
                int start_pos = num_iterations*i;
                retval = pthread_create(&threads[i], NULL, &doTaskWithMutex, &start_pos);
                if ( retval != 0 ) {  // Error handling
                    fprintf(stderr, "Error: could not create requested number of threads.\n");
                    exit_status = 1;
                    goto error;
                }
            }
            break;
        case 's':   // Spin-lock
            for ( i = 0; i < num_threads; i++ ) {
                int start_pos = num_iterations*i;
                retval = pthread_create(&threads[i], NULL, &doTaskWithSpinLock, &start_pos);
                if ( retval != 0 ) {  // Error handling
                    fprintf(stderr, "Error: could not create requested number of threads.\n");
                    exit_status = 1;
                    goto error;
                }
            }
            break;
        default:    // Error
            fprintf(stderr, "Error: invalid sync value\n");
            exit_status = 1;
            goto error;
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
    long long unsigned num_ops = num_threads*num_iterations*num_iterations;
    long long unsigned time_per_op = total_time/num_ops;
    for ( k = 0; k < opt_lists; k++ )
        result_length += SortedList_length(list_head[k]);

    // Print summary of results
    printf("%d threads x %d iterations x (ins + lookup/del) x (%d/2 avg len) = %llu operations\n", num_threads, num_iterations, num_iterations, num_ops);
    if ( result_length != 0 )
        fprintf(stderr, "ERROR: final count = %d\n", result_length);
    else
        printf("final count = %d\n", result_length);
    printf("elapsed time: %llu ns\n", total_time);
    printf("per operation: %llu ns\n", time_per_op);

    error:
    // Free key memory for each element, then free elements array
    if ( elements ) {
        for ( i = 0; i < total_elements; i++ ) {
            if ( (elements+i)->key )
                free((void*) (elements+i)->key);
        }
        free((void*) elements);
    }
    // Free list_head and threads array if not NULL
    for ( k = 0; k < opt_lists; k++ ) {
        if ( list_head[k] )
            free((void*) list_head[k]);
    }
    if ( threads )
        free((void*) threads);

    exit(exit_status);
}

//******************************************************************************
// Wrapper functions
//******************************************************************************

// Function wrapper for add with no synchronization
void * doTask(void *start_pos) {
    int i, k;
    int offset = *((int*)start_pos);
    int list_index = 0;
    // Perform requested number of inserts
    for ( i = offset; i < num_iterations + offset; i++ ) {
        list_index = findhash((elements+i)->key);
        SortedList_insert(list_head[list_index], elements+i);
    }
    
    // Check list length (as required by spec)
    for ( k = 0; k < opt_lists; k++ ) {
        printf("%d\n", SortedList_length(list_head[k]));
        if ( SortedList_length(list_head[k]) < 0 )
            return (void*)-1;
    }

    // Perform requested number of lookups/deletes
    for ( i = offset; i < num_iterations + offset; i++ ) {
        list_index = findhash((elements+i)->key);
        SortedListElement_t *ele_to_delete = SortedList_lookup(list_head[list_index], (elements+i)->key);
        // Check that element with key was found before deleting
        if ( ele_to_delete ) {
            if ( SortedList_delete(ele_to_delete) == 1 )
                return (void*)-1;
        }
        else
            return (void*)-1;
    }
    return (void*)0;
}

// Function wrapper for add with mutex synchronization
void * doTaskWithMutex(void *start_pos) {
    int i, k;
    int offset = *((int*)start_pos);
    int list_index = 0;
    static pthread_mutex_t mutex;

    pthread_mutex_lock(&mutex);
    // Perform requested number of inserts
    for ( i = offset; i < num_iterations + offset; i++ ) {
        list_index = findhash((elements+i)->key);
        SortedList_insert(list_head[list_index], elements+i);
    }

    // Check list length (as required by spec)
    for ( k = 0; k < opt_lists; k++ ) {
        if ( SortedList_length(list_head[k]) < 0 ) {
            pthread_mutex_unlock(&mutex);
            return (void*)-1;
        }
    }

    // Perform requested number of lookups/deletes
    for ( i = offset; i < num_iterations + offset; i++ ) {
        list_index = findhash((elements+i)->key);
        SortedListElement_t *ele_to_delete = SortedList_lookup(list_head[list_index], (elements+i)->key);
        // Check that element with key was found before deleting
        if ( ele_to_delete ) {
            if ( SortedList_delete(ele_to_delete) == 1 ) {
                pthread_mutex_unlock(&mutex);
                return (void*)-1;
            }
        }
        else {
            pthread_mutex_unlock(&mutex);
            return (void*)-1;
        }
    }
    pthread_mutex_unlock(&mutex);
    return (void*)0;
}

// Function wrapper for add with spin lock synchronization
void * doTaskWithSpinLock(void *start_pos) {
    int i, k;
    int offset = *((int*)start_pos);
    int list_index = 0;
    static volatile int spinlock = 0;

    while ( __sync_lock_test_and_set(&spinlock, 1) );
    // Perform requested number of inserts
    for ( i = offset; i < num_iterations + offset; i++ ) {
        list_index = findhash((elements+i)->key);
        SortedList_insert(list_head[list_index], elements+i);
    }
    
    // Check list length (as required by spec)
    for ( k = 0; k < opt_lists; k++ ) {
        if ( SortedList_length(list_head[k]) < 0 ) {
            __sync_lock_release(&spinlock);
            return (void*)-1;
        }
    }

    // Perform requested number of lookups/deletes
    for ( i = offset; i < num_iterations + offset; i++ ) {
        list_index = findhash((elements+i)->key);
        SortedListElement_t *ele_to_delete = SortedList_lookup(list_head[list_index], (elements+i)->key);
        // Check that element with key was found before deleting
        if ( ele_to_delete ) {
            if ( SortedList_delete(ele_to_delete) == 1 ) {
                __sync_lock_release(&spinlock);
                return (void*)-1;
            }
        }
        else {
            __sync_lock_release(&spinlock);
            return (void*)-1;
        }
    }
    __sync_lock_release(&spinlock);
    return (void*)0;
}

//******************************************************************************
// Sorted list functions
//******************************************************************************

SortedList_t* SortedList_new(void) {
    SortedList_t *new_list = (SortedList_t*) malloc(sizeof(SortedList_t));
    if ( !new_list ) {
        fprintf(stderr, "Error SortedList_new: unable to allocate memory.\n");
        return NULL;
    }
    new_list->prev = new_list;
    new_list->next = new_list;
    new_list->key = NULL;
    return new_list;
}

void SortedList_insert(SortedList_t *list, SortedListElement_t *element) {
    SortedListElement_t *ele_prev = list;
    SortedListElement_t *ele_next = list->next;
    int count = 0;
    while ( ele_next != list ) {
        // Compare keys and break if found target location (sorted ascending order)
        if ( strcmp(element->key, ele_next->key) <= 0 )
            break;
        // Go to next element in list
        ele_prev = ele_next;
        ele_next = ele_next->next;
        count++;
        if ( count > num_iterations * num_threads ) {
            fprintf(stderr, "Error: list corruption detected!\n");
            exit(1);
        }
    }
    if ( opt_yield & SEARCH_YIELD )
        pthread_yield();
    // Found location, hook up pointers between current elements
    element->next = ele_next;
    element->prev = ele_prev;
    ele_next->prev = element;
    ele_prev->next = element;
}

int SortedList_delete( SortedListElement_t *element) {
    SortedListElement_t *ele_prev = element->prev;
    SortedListElement_t *ele_next = element->next;
    // Check pointers are valid
    if ( ele_next->prev != element )
        return 1;
    if ( ele_prev->next != element )
        return 1;
    if ( opt_yield & DELETE_YIELD )
        pthread_yield();
    // Rearrange pointers and set element pointers to NULL
    ele_next->prev = element->prev;
    ele_prev->next = element->next;
    element->prev = NULL;
    element->next = NULL;
    return 0;
}

SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key) {
    SortedListElement_t *element = list->next;
    int count = 0;
    while ( element && element != list ) {
        // Check if element has key
        if ( element->key == key )
            return element;
        element = element->next;
        count++;
        // Detect infinite loop
        if ( count > num_iterations * num_threads ) {
            fprintf(stderr, "Error: list corruption detected!\n");
            exit(1);
        }
        if ( opt_yield & SEARCH_YIELD )
            pthread_yield();
    }
    // Didn't find key
    return NULL;
}

int SortedList_length(SortedList_t *list) {
    SortedListElement_t *element = list;
    int count = 0;
    while ( element->next != list ) {
        // Check for NULL pointers
        if (!element->next || !element->prev)
            return -1;
        // Increment count
        count++;
        // Detect infinite loop
        if ( count > num_iterations * num_threads ) {
            fprintf(stderr, "Error: list corruption detected!\n");
            exit(1);
        }
        element = element->next;
        if ( opt_yield & SEARCH_YIELD )
            pthread_yield();
    }
    return count;
}

//******************************************************************************
// Helper functions
//******************************************************************************

// Takes a key and calculates a hash value via string folding and the number
// of SortedLists needed to distribute across
int findhash(const char *key) {
    int sum = 0;
    for ( int i = 0; i < key_length; i++ )
        sum += key[i];
    return sum % opt_lists;
}