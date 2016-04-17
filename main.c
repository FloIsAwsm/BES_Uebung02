/**
 * @brief this is a simple and short test for our mypopen and mypclose functions
*/

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "mypopen.h"

int main(int argc, char * argv[])
{
	argc = argc;
	argv = argv;

	FILE * fp = mypopen("ls -l", "r");
	if(fp == NULL)
	{
		printf("%s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	
	if(mypclose(fp) != EXIT_SUCCESS)
	{
		printf("%s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}