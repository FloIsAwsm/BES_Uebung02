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

	/* test invalid parameters */
	FILE * retVal;

	retVal = mypopen(NULL, NULL);
	if (retVal != NULL)
	{
		printf("something went wrong here... mypopen(NULL, NULL);\n");
		return EXIT_FAILURE;
	}

	retVal = mypopen("ls", NULL);
	if (retVal != NULL)
	{
		printf("something went wrong here... mypopen(\"ls\", NULL);\n");
		return EXIT_FAILURE;
	}

	retVal = mypopen(NULL, "r");
	if (retVal != NULL)
	{
		printf("something went wrong here... mypopen(NULL, \"r\");\n");
		return EXIT_FAILURE;
	}

	retVal = mypopen("ls", "rw");
	if (retVal != NULL)
	{
		printf("something went wrong here... mypopen(\"ls\", \"rw\");\n");
		return EXIT_FAILURE;
	}

	retVal = mypopen("ls", "wr");
	if (retVal != NULL)
	{
		printf("something went wrong here... mypopen(\"ls\", \"wr\");\n");
		return EXIT_FAILURE;
	}

	retVal = mypopen("ls", "");
	if (retVal != NULL)
	{
		printf("something went wrong here... mypopen(\"ls\", \"\");\n");
		return EXIT_FAILURE;
	}

	FILE * fp = mypopen("ls -l", "r");
	if(fp == NULL)
	{
		printf("%s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	/*
	if(mypclose(fp) != EXIT_SUCCESS)
	{
		printf("%s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	*/

	return EXIT_SUCCESS;
}