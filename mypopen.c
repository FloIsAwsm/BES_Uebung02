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
 * @todo rework function comments
 * @todo resolve todos
 * @todo comment includes to show why we need them
 * @todo add an include for FILE * in mypopen.h
 * @todo look over comments
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
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
 * File pointer to the current open steam
 */
static FILE * _fp = NULL;

/**
 * the process id of the child
 */
static int pid = 0;

/**
 * @brief handles the childprocess after fork 
 * @details todo
 * 
 * @param command the command later passed to the shell
 * @param type the direction of the pipe
 */
static void fork_child(const char * command, const char * type);

/**
 * @brief handles the parent process after fork
 * @details todo
 * 
 * @param type defines the direction of the pipe to be returned
 * @return returnes a FILE pointer to the pipe stream
 */
static FILE * fork_parent(const char * type);

FILE * mypopen(const char * command, const char * type)
{
	// input validation
	// @todo make this a litte easier to read
	if (command == NULL || type == NULL || strlen(type) != 1 || (!(type[0] == 'w' || type[0] == 'r')))
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
		// errno gets set by pipe
		return NULL;
	}

	// fork
	pid = fork();

	if (pid == EXIT_ERROR)
	{
		// errno gets set by fork
		
		// reset pid to avoid problems in mypclose()
		pid = 0;

		if(close(fd[PIPE_WRITE]) == EXIT_ERROR)
		{
			// @todo now what?
		}
		if(close(fd[PIPE_READ]) == EXIT_ERROR)
		{
			// @todo now what?
		}
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
		// @todo do I have to close FILE * stream?
		errno = ECHILD;
		return EXIT_ERROR;
	}

	if (stream == NULL)
	{
		// @todo do I have to close _fp?
		errno = EINVAL;
		return EXIT_ERROR;
	}

	if (stream != _fp)
	{
		// @todo do I have to close stream and _fp?
		errno = EINVAL;
		return EXIT_ERROR;
	}

	if (fclose(_fp) != 0)
	{
		_fp = NULL;
	    return EXIT_ERROR;
	}

	_fp = NULL;
	/* @todo I feel like we have to close more here... */

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

/* @todo how do we handle errors after the first one. we will not get them in our application */
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
		if (close(fd[PIPE_WRITE]) == EXIT_ERROR)
		{
			// @todo what errno value do we return here
		}
		if (close(fd[PIPE_READ]) == EXIT_ERROR)
		{
			// @todo what errno value do we return here
		}
		exit(EXIT_FAILURE);
	}
	if(close(fd[PIPE_WRITE]) == EXIT_ERROR)
	{
		if(close(fd[PIPE_READ]) == EXIT_ERROR)
		{
			// @todo do we need to check for an error here?
		}
		exit(EXIT_FAILURE);
	}
	if(close(fd[PIPE_READ]) == EXIT_ERROR)
	{
		// @todo do we try calling close again?
		exit(EXIT_FAILURE);
	}

	// no begin executing the command
	if (execl("/bin/sh", "sh", "-c", command, NULL) == EXIT_ERROR)
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
		if(close(fd[_toOpen]) == EXIT_ERROR)
		{
			// @todo do we need that if here?
		}
		return NULL;
	}

	_fp = fdopen(fd[_toOpen], type);
	if(_fp == NULL)
	{
		if(close(fd[_toOpen]) == EXIT_ERROR)
		{
			// @todo what errno value do we need here?
		}
	}
	return _fp;
}
