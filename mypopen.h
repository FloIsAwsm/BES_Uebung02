/**
 * @file mypopen.h
 * 
 * Beispiel 2
 * 
 * @author Florian Froestl 	<florian.froestl@technikum-wien.at>
 * @author David Boisits	<david.boisits@technikum-wien.at>
 * 
 * @date 2016/04/10
 * 
 * @version 100
 * 
 */
#include <stdio.h>
#include <stdlib.h>

#ifndef MYPOPEN_H
#define MYPOPEN_H

/**
 * @brief success return code
 * @detais defines the return code for a successful operation
 */
#define EXIT_SUCCESS 0

/**
 * @brief failure return code
 * @detais defines the return code for a failed operation or an error
 */
#define EXIT_FAILURE 1

/**
 * @brief error return code
 * @details defines the return code for an error from which we cannot recover.
 * This is always the case if the input arguments are wrong. (e.g.: ./myfind . -type xy)
 */
#define EXIT_ERROR -1

/**
 * @brief
 * @details
 * 
 * @param 
 * 
 * @return EXIT_SUCCESS on success, EXIT_FAILURE otherwise
 */
FILE *mypopen(const char *command, const char *type);
int mypclose(FILE *fpc);

#endif
