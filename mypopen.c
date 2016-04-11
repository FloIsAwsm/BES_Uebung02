/**
* @file mypopen.c
* Beispiel 2
*
* @author David Boisits <ic15b041@technikum-wien.at>
* @author Florian Fröstl
* @date 2016/04/10
*
* @version $Revision: 1.0 $
*
* URL: $HeadURL$
*
* Last Modified: $Author: $
*/

#include "mypopen.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

/* Global process ID */
static pid_t pid;
/* Global File Pointer */
static FILE *fp = NULL;

/* fork -1 = fehler; >0 = Parent Process; fork == 0 child process */

/* mypopen function */
FILE *mypopen(const char *command, const char *type)
{
	/* File descriptor Index für FD-Table; 0=STDIN; 1=STDOUT; 2=STDERR; */
	int fd[2];
	
	if(fp != NULL)
	{
		errno = EAGAIN;
		exit(EXIT_FAILURE);
		
		/* type must be r or w */
		/* command must be greater than 0 */
		if(strlen(type) != 1 || command == NULL)
		{
			errno = EINVAL;
			exit(EXIT_FAILURE);
		}		
		if(type[0] != 'r' || type[0] != 'w')
		{
			errno = EINVAL;
			exit(EXIT_FAILURE);
		}
		
		if(pipe(fd) == -1)
		{
			exit(EXIT_FAILURE);
		}
		
		if((pid = fork()) == -1)
		{
			close(fd[1]);
			close(fd[0]);
			exit(EXIT_FAILURE);
		}
		
		/* Read Mode */
		if(type[0] == 'r')
		{
			/* Child Process */
			if(pid == 0)
			{
				/* dup2 = Duplicate old fd to new fd */
				if(dup2(fd[1], 1) == -1)
				{
					close(fd[1]);
					exit(EXIT_FAILURE);
				}
				/* close both pipe-ends */
				close (fd[1]);
				
				/* execute the shell command */
				execl("/bin/bash", "sh", "-c", command, NULL);
				
				exit(EXIT_FAILURE);
			}
			/* Parent Process */
			else
			{
				/* close the write-end */
				close(fd[1]);
				
				if((fp = fdopen(fd[0], "r")) == NULL)
				{
					close(fd[0]);
					return NULL;
				}
			}
		}
		/* Write Mode */
		else if(type[0] == 'w')
		{
			/* Child Process */
			if(pid == 0)
			{
			/* dup2 = Duplicate old fd to new fd */
				if(dup2(fd[0], 0) == -1)
				{
					close(fd[0]);
					exit(EXIT_FAILURE);
				}
				
				/* Close both pipe ends */
				close(fd[0]);
				
				execl("/bin/bash", "sh", "-c", command, NULL);
				
				exit(EXIT_FAILURE);
			}
			else
			{
				close(fd[0]);
				
				/* open for writting */
				if((fp = fdopen(fd[1], "w")) == NULL)
				{
					close(fd[1]);
					return NULL;
				}
			}
		}
		else
		{
			exit(EXIT_FAILURE);
		}
		
		return fp;
	}
	return NULL;
}

int mypclose(FILE *fpc)
{
	pid_t wpid;
	int status = 0;
	
	if(fpc == NULL)
	{
		errno = EINVAL;
		return EXIT_ERROR;
	}
	
	if(fp == NULL)
	{
		errno = ECHILD;
		return EXIT_ERROR;
	}
	
	if(fpc != fp)
	{
		errno = EINVAL;
		return EXIT_ERROR;
	}
	
	if(fclose(fp) != 0)
	{
		return EXIT_ERROR;
	}
	
	fp = NULL;
	
	errno = 0;
	while((wpid = waitpid(pid, &status, 0)) != pid)
	{
		if(wpid == -1)
		{
			if(errno == EINTR)
			{
				continue;
			}
			return EXIT_ERROR;
		}
	}	
	return NULL;
}





















