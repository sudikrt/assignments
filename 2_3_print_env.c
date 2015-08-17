#include <stdio.h>

/* The environ variable conatains environment values. */
extern char** environ;

int main ()
{
	char** var;
	for (var = environ; *var != NULL; ++var)
	{
		printf ("%s\n", *var);
		sleep(1);
	}	
	return 0;
}
