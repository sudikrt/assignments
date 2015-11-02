#include <stdio.h>

int main (int argc, char* argv[])
{
	printf ("Program name : %s.\n",argv[0]);
	printf ("Total no of args : %d.\n", argc - 1);
	
	//check if more than one arguments
	if(argc > 1)
	{
		int i;
		printf ("The arguments are \n");
		for( i = 1; i < argc; ++i)
			printf ("%s\n", argv[i]);
	}	
}
