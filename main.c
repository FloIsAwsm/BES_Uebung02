/**
 * @brief this is a simple and short test for our mypopen and mypclose functions
*/

#include <stdio.h>

int main(int argc, char * argv[])
{

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


	return EXIT_SUCCESS;
}