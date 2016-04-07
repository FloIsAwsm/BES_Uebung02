/**
 * @todo insert file header
 * @todo implement functions
 */

#include "mypopen.h"

FILE * mypopen(const char * command, const char * type)
{
	return popen(command, type);
}

int mypclose(FILE * stream)
{
	return pclose(stream);
}