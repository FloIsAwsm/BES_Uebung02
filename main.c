/**
 * @file main.c
 * 
 * Beispiel 2
 * 
 * @author Florian Froestl <florian.froestl@technikum-wien.at>
 * @author David Boisits <david.boisits@technikum-wien.at>
 * 
 * @date 2016/04/10
 * 
 * @version 100
 * 
 * @todo
 */
#include <stdio.h>
#include "mypopen.h"

int main(int argc, char * argv[])
{
	
	argc = argc;
	argv = argv;
	
	mypopen("ls", "r");
	
	return EXIT_SUCCESS;
}
