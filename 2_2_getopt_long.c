#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>

/*Name of the program*/
const char* program_name;

/*Prints usage information of this program to STREAM
and exit the program with an EXIT_CODE*/

void print_usage(FILE* stream, int exit_code)
{
	fprintf (stream, "Usage: %s optioo [ inputfile ... ]\n", program_name);
	fprintf (stream, " -h --help             Display the usage information.\n"
			 " -o --output filename  write the output to file.\n"
			 " -v --verbose          Print the verbose messages.\n");
	exit (exit_code);
}

/*Main program entry point*/

int main (int argc, char* argv[])
{
	int next_option;
	
	/* A String listing valid short option letters. */
	const char* const short_options = "ho:v";

	/* An array describing valid lont options. */
	const struct option long_options[] = {
		{ "help",	0, NULL, 'h' },
		{ "output",	1, NULL, 'o' },
		{ "verbose",	0, NULL, 'v' },
		{ "NULL",	0, NULL,  0  }  /* Required at the end of the array */
	};
	 
	/* The name of the file to recive program output, or NULL for
	   standard output. */
	const char* output_filename = NULL;
	/* Whether to display verbose messages. */
	int verbose = 0;

	/* Remember the name of the program to incorporate in messages.
	   The name is stored in argv[0]. */
	program_name = argv[0];
	
	do {
		next_option = getopt_long (argc, argv, short_options, 
					   long_options, NULL);
		switch (next_option)
		{
		case 'h':	/* -h or --help */
			/* User has requested usage information. print it to standard
			   output, and exit with exit code zero (normal termination). */
			print_usage (stdout,0);
		case 'o':	/* -o or --output */
			/* This option takes an argument, the name of the output file */
			output_filename = optarg;
			break;
		case 'v':	/* -v or --verbose*/
			verbose = 1;
			break;
		case '?':	/* The user specified an invalid option. */
			/* Print usage information to standard error, and exit with an exit 
			   code one  (indication abnormal termination). */
			print_usage (stderr,1);
		case -1:	/* Done with options. */
			break;
		default:	/* Something else: unexpected. */
			abort();
		}
	}
	while (next_option != -1);
	 
	/* Done with the options. OPTIND points to first nonoption argument.
	   For demonstration purposes, print them if the verbose option was 
	   specified. */
	
	if (verbose)
	{
		int i;
		for (i = optind; i < argc; ++i)
			printf ("Argument: %s\n", argv[i]);
	}
	
	return 0;
}	
