/**
 * @file mypopen.h
 * 
 * Beispiel 2
 * 
 * @author Florian Froestl <florian.froestl@technikum-wien.at>
 * @author David Boisits <david.boisits@technikum-wien.at>
 * 
 * @date 2016/04/16
 * 
 * @version 100
 * 
 */

#ifndef MYPOPEN_H
#define MYPOPEN_H

/**
 * exit value if the function was successful
 */
#define EXIT_SUCCESS 0

/**
 * exit value if the function was not successful
 */
#define EXIT_FAILURE 1
/**
 * return value in case of an error
 */
#define EXIT_ERROR  -1

/**
 * @brief popen clone
 * @details opens a pipe then creates a child process and replaces stdin or stdout depending on the type parameter
 * in the child process. the parent process returns the other end of the pipe as a file pointer
 * 
 * @param command shell command to execute in the child process
 * @param type specifies the desired end of the pipe to be returned. should not be anything other than "r" or "w"
 * 
 * @return file pointer of the pipe
 */
FILE * mypopen(const char * command, const char * type);

/**
 * @brief pclose clone
 * @details closes the pipe stream and waits for the child to finish it's operation
 * to return the childs exit status
 * 
 * @param stream file pointer to the parents pipe end
 * @return exit status of the child process or -1 in case of an error
 */
int mypclose(FILE * stream);

#endif