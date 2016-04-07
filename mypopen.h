/**
 * @todo insert file header
 * @todo document functions
 */

#ifndef MYPOPEN_H
#define MYPOPEN_H

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#include <stdio.h>

/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param command [description]
 * @param type [description]
 * 
 * @return [description]
 */
FILE * mypopen(const char * command, const char * type);

/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param stream [description]
 * @return [description]
 */
int mypclose(FILE * stream);

#endif