#include <stdlib.h> // exit
#include <stdio.h> 	// printf

#include <unistd.h> // getopt_long
#include <getopt.h> // struct option (longopts)

int main(int argc, char **argv)
{

	static struct option long_options[] =
    {
    	{"iterations", required_argument, 0, 'i' },
        {0, 0, 0, 0}
    };

    if (argc <= 1) {		// No arguements
        printf("usage: %s [--threads=#] [--iterations=#] [--iter=#] [--yield=#] [--sync=[msc]]\n", argv[0]);
    } else if (argc > 1) { 	// At least one argument

    }

    exit(0);
}