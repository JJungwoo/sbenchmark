#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main()
{
	int fd[2];
	int i, n, state;
	char buf[256];
	
	struct timeval tv;
	fd_set readfds, writefds;

	if (!(fd[0] = open("/tmp/testfile1", O_RDONLY)))
	{
		perror("file1 open error : ");
		exit(0);
	}

	if (!(fd[1] = open("/tmp/testfile2", O_RDONLY)))
	{
		perror("file2 open error : ");
		exit(0);
	}

	memset (buf, 0x00, sizeof(buf));

	for(;;)
	{
		FD_ZERO(&readfds);
		FD_SET(fd[0], &readfds);
		FD_SET(fd[1], &readfds);
		
		state = select(fd[1]+1, &readfds, NULL, NULL, NULL);
		
		switch(state)
		{
			case -1:
				perror("select error : ");
				exit(0);
				break;
			default:
				for (i = 0 ; i < 2 ; i++)
				{
					if (FD_ISSET(fd[1], &readfds))
					{
						while ((n = read(fd[i], buf, sizeof(buf))) > 0)
							printf("(%d) [%d] %s\n", state, i, buf);
					}
				}
				memset (buf, 0x00, sizeof(buf));
				break;
		}
		//usleep(1000);
	}	
}	
