#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int fork_bomb(int level);

int main(int argc, char ** argv)
{
	argc = argc;
	argv = argv;

	fork_bomb(0);

	return 0;
}

int fork_bomb(int level)
{
	if(level > 0)
	{
		printf("still alive at level: %d\n", level);
	}
	usleep(500000);
	int retVal = fork();

	if(retVal == -1)
	{
		// nooooooes
		exit(1);
	}
	else if(retVal == 0)
	{
		fork_bomb(level+1);
	}
	else
	{
		int r = 0;
		waitpid(retVal, &r, 0);
	}
	return 0;
}