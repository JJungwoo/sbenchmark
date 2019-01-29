#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>
#include <signal.h>

#define BUF_SIZE 1024
#define TIMEOUT 2

int main()
{
	struct timeval tv;
	fd_set readfds, tmpfds;
	int ret;
	char buf[BUF_SIZE];
	int len;
	int fd;

	if (-1 == (fd = open("/tmp/testfile1", O_RDONLY))) {
		perror("file1 open error: ");
		exit(1);
	}

	FD_ZERO(&readfds);
	FD_SET(STDIN_FILENO, &readfds);

	FD_SET(fd, &readfds);

	printf("while before fd isset: %d, %d \n", fd, FD_ISSET(fd, &readfds));
	while (1) {
		tmpfds = readfds;
		
		print_fd(tmpfds);
		printf("fd isset: %d, %d \n", fd, FD_ISSET(fd, &readfds));
		tv.tv_sec = TIMEOUT;
		tv.tv_usec = 0;
		puts("select bf");
		ret = select(STDIN_FILENO + 1, &tmpfds, 0, 0, &tv);
		puts("select af");

		if (-1 == ret)	
			perror("select() error");
		else if (0 == ret)
			printf("%d counting...\n", TIMEOUT);
		else{
			if (FD_ISSET(STDIN_FILENO, &readfds)) {
				len = read(STDIN_FILENO, buf, BUF_SIZE);
				if (-1 == len) {
					perror("read() error");
					exit(1);
				}
				if (len) {
					buf[len] = '\0';
					printf("read: %s\n", buf);
				}

			}
		}

	}	

	return 0;
}

void print_fd(fd_set fds)
{
	int i;
	for(i=0;i<10;i++)
	{
		printf("%d: %d \n", i, FD_ISSET(i, &fds));
	}
}
