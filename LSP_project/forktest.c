#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main()
{
	int pid, i;
	
	i = 1000;
	pid = fork();
	
	if (-1 == pid)
	{
		perror("fork error ");
		exit(0);
	}
	else if (0 == pid)
	{
		printf(" (child) my pid is %d \n", getpid());
		while(1)
		{
			printf("--> %d \n", i);
			i++;
			sleep(1);
		}
	}
	else
	{
		printf(" (parent) my child pid is %d \n", pid);
		sleep(1);
		printf(" (parent is dead)\n");
		exit(0);
	}
}
