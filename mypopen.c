/**
 * @file mypopen.c
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

#include <stdlib.h> // exit
#include <unistd.h> // dup2, STDIN_FILENO...
#include <errno.h> // errno, EINVAL...
#include <string.h> //strlen
#include <sys/wait.h> // waitpid
#include "mypopen.h"

/**
 * defines the location of the read end of the pipe in the Filedescritor array
 */
#define PIPE_READ 0

/**
 * defines the location of the write end of the pipe in the Filedescritor array
 */
#define PIPE_WRITE 1

/** 
 * size of the File Descriptor array 
 */
#define file_desc_arr_size 2

/**
 * File Descriptor array
 */
static int fd[file_desc_arr_size];

/**
 * File pointer to the current open stream
 */
static FILE * _fp = NULL;

/**
 * the process id of the child
 */
static int pid = 0;

/**
 * @brief handles the childprocess after we fork 
 * @details replaces STDIN or STDOUT with the correct end of our pipe and
 * executes the command. If an error occurs the child proccess calls exit.
 * 
 * @param command the command later passed to the shell with an execl call
 * @param type the direction of the pipe
 */
static void fork_child(const char * command, const char * type);

/**
 * @brief handles the parent process after we fork
 * @details opens the file descriptor using fdopen and returns it
 * 
 * @param type defines the direction of the pipe to be returned
 * 
 * @return returnes a FILE pointer to the pipe stream or NULL if an error occurs
 */
static FILE * fork_parent(const char * type);

FILE * mypopen(const char * command, const char * type)
{
	// input validation

	// start off by testing for NULL pointers
	if (command == NULL || type == NULL)
	{
		errno = EINVAL;
		return NULL;
	}

	// if the type string size is not valid
	if(strlen(type) != 1)
	{
		errno = EINVAL;
		return NULL;
	}

	// if type is neither "w" or "r"
	if(*type != 'w' && *type != 'r')
	{
		errno = EINVAL;
		return NULL;
	}

	// check if pipe is already open
	if(_fp != NULL)
	{
		errno = EAGAIN;
		return NULL;
	}

	// open pipe
	if(pipe(fd) != 0)
	{
		// errno get s set by pipe
		return NULL;
	}

	// fork
	pid = fork();

	if (pid == EXIT_ERROR)
	{
		// save errno if it is overwritten by the close() calls
		int _err = errno;
		
		(void) close(fd[PIPE_WRITE]);
		(void) close(fd[PIPE_READ]);

		errno = _err;

		return NULL;
	}
	else if(pid == 0)
	{
		fork_child(command, type);
	}
	else
	{
		return fork_parent(type);
	}

	return NULL;
}

int mypclose(FILE * stream)
{
	if (_fp == NULL)
	{
		errno = ECHILD;
		return EXIT_ERROR;
	}

	if (stream == NULL)
	{
		errno = EINVAL;
		return EXIT_ERROR;
	}

	if (stream != _fp)
	{
		errno = EINVAL;
		return EXIT_ERROR;
	}

	if (fclose(_fp) != 0)
	{
		_fp = NULL;
	    return EXIT_ERROR;
	}

	_fp = NULL;

    int status = 0;
	int wpid = 0;

	while((wpid = waitpid(pid, &status, 0)) != pid)
	{
		if(wpid == EXIT_ERROR)
		{
			if(errno == EINTR)
			{
				continue;
			}
			return EXIT_ERROR;
		}
	}
	pid = 0;

	if(WIFEXITED(status))
	{
		// return exit status of child
		return WEXITSTATUS(status);
	}
	else
	{
		errno = ECHILD;
		return EXIT_ERROR;
	}
}

static void fork_child(const char * command, const char * type)
{
	int _toDup;
	int _stdFd;

	if (type[0] == 'w')
	{
		_toDup = PIPE_READ;
		_stdFd = STDIN_FILENO;
	}
	else /* type == "r" */
	{
		_toDup = PIPE_WRITE;
		_stdFd = STDOUT_FILENO;
	}

	if(dup2(fd[_toDup], _stdFd) == EXIT_ERROR)
	{
		int _err = errno;

		(void) close(fd[PIPE_WRITE]);
		(void) close(fd[PIPE_READ]);
		
		errno = _err;
		exit(EXIT_FAILURE);
	}
	if(close(fd[PIPE_WRITE]) == EXIT_ERROR)
	{
		int _err = errno;
		(void) close(fd[PIPE_READ]);

		errno = _err;
		exit(EXIT_FAILURE);
	}
	if(close(fd[PIPE_READ]) == EXIT_ERROR)
	{
		exit(EXIT_FAILURE);
	}

	// no begin executing the command
	if (execl("/bin/sh", "sh", "-c", command, (char*) NULL) == EXIT_ERROR)
    {
        exit(EXIT_FAILURE);
    }
    /* we should never reach this point */
}

static FILE * fork_parent(const char * type)
{
	int _toClose;
	int _toOpen;
	if(type[0] == 'w')
	{
		_toOpen = PIPE_WRITE;
		_toClose = PIPE_READ;
	}
	else /* type == "r" */
	{
		_toOpen = PIPE_READ;
		_toClose = PIPE_WRITE;	
	}

	if(close(fd[_toClose]) == EXIT_ERROR)
	{
		int _err = errno;
		(void) close(fd[_toOpen]);
		errno = _err;
		return NULL;
	}

	_fp = fdopen(fd[_toOpen], type);
	if(_fp == NULL)
	{
		int _err = errno;
		(void) close(fd[_toOpen]);
		errno = _err;
	}
	return _fp;
}
