#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	int pp[2];
	int pid;
	char *ps = "test pipe line";

	if (0 > pipe(pp))
		printf("pipe error: ");
	pid = fork();
	if (0 > pid) 
	{
		perror("fork error");
		exit(0);
	}
	if (0 == pid)
	{
		close(pp[1]);
		dup2(pp[0], 0);
		if (0 > execl("/home/jungwoo/jjo/LSP_project/pipe_cl", "pipe_cl", 0))
			perror("execl error");
	}
	else if (0 < pid)
	{
		close(pp[0]);
		while(1)
		{
			sleep(1);
			printf("Write\n");
			//write(pp[1], (void *)&pid, sizeof(pid));
			write(pp[1], ps, sizeof(ps));
		}
	}
}
	
	
