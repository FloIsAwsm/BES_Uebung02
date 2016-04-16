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
 * @brief defines the location of the read end of the pipe in the Filedescritor array
 */
#define PIPE_READ 0

/**
 * @brief defines the location of the write end of the pipe in the Filedescritor array
 */
#define PIPE_WRITE 1

/**
 * @brief size of the File Descriptor array
 */
#define file_desc_arr_size 2

/**
 * @brief File Descriptor array
 */
static int fd[file_desc_arr_size];

/**
 * @brief File pointer to the current open steam
 */
static FILE * _fp = NULL;

/**
 * @brief the process id of the child
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
		close(fd[PIPE_WRITE]);
		close(fd[PIPE_READ]);
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

	if (fclose(_fp) == EOF)
    {
    	_fp = NULL;
        return (EXIT_ERROR);
    }

    _fp = NULL;
    /* @todo I feel like we have to close more here... */

    int status = 0;

	int wpid = 0;
	errno = 0;
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
	if (type[0] == 'w')
	{
		if(dup2(fd[PIPE_READ], STDIN_FILENO) == EXIT_ERROR)
		{
			if (close(fd[PIPE_WRITE]) == EXIT_ERROR)
			{
				// @todo what errno value do we return here
			}
			if (close(fd[PIPE_READ]) == EXIT_ERROR)
			{
				// @todo what errno value do we return here
			}
			exit(1);
		}
		if(close(fd[PIPE_WRITE]) == EXIT_ERROR)
		{
			if(close(fd[PIPE_READ]) == EXIT_ERROR)
			{
				// @todo do we need to check for an error here?
			}
			exit(1);
		}
		if(close(fd[PIPE_READ]) == EXIT_ERROR)
		{
			// @todo do we try calling close again?
			exit(1);
		}
	}
	else
	{
		// stdout
		if(dup2(fd[PIPE_WRITE], STDOUT_FILENO) == EXIT_ERROR)
		{
			if (close(fd[PIPE_WRITE]) == EXIT_ERROR)
			{
				// @todo what errno value do we return here
			}
			if (close(fd[PIPE_READ]) == EXIT_ERROR)
			{
				// @todo what errno value do we return here
			}
			exit(1);
		}
		if(close(fd[PIPE_READ]) == EXIT_ERROR)
		{
			if(close(fd[PIPE_READ]) == EXIT_ERROR)
			{
				// @todo do we need to check for an error here?
			}
			exit(1);
		}

		if(close(fd[PIPE_WRITE]) == EXIT_ERROR)
		{
			// @todo do we try calling close again?
			exit(1);
		}
	}

	// no begin executing the command
	if (execl("/bin/sh", "sh", "-c", command, NULL) == EXIT_ERROR)
    {
        exit(1);
    }

    /* we never reach this */
}

static FILE * fork_parent(const char * type)
{
	if(type[0] == 'w')
	{
		if(close(fd[PIPE_READ]) == EXIT_ERROR)
		{
			if(close(fd[PIPE_WRITE]) == EXIT_ERROR)
			{
				// @todo do we need that if here?
				return NULL;
			}
			return NULL;
		}
		_fp = fdopen(fd[PIPE_WRITE], type);
		if(_fp == NULL)
		{
			if(close(fd[PIPE_WRITE]) == EXIT_ERROR)
			{
				// @todo what errno value do we need here?
				return NULL;
			}
			return NULL;
		}
		return _fp;
	}
	else
	{
		if(close(fd[PIPE_WRITE]) == EXIT_ERROR)
		{
			if(close(fd[PIPE_READ]) == EXIT_ERROR)
			{
				// @todo do we need that if here?
				return NULL;
			}
			return NULL;
		}

		_fp = fdopen(fd[PIPE_READ], type);
		if(_fp == NULL)
		{
			if(close(fd[PIPE_READ]) == EXIT_ERROR)
			{
				// @todo what errno value do we need here?
				return NULL;
			}
			return NULL;
		}
		return _fp;
	}
}
