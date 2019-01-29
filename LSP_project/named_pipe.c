#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define MSG_SIZE 80

int main()
{
	char msg[MSG_SIZE];
	int f_des;
	int nread, cnt;

	if (-1 == mkfifo("./fifo", 0666)) {
		printf("fail to call fifo() \n");
		exit(1);
	}

	if (0 > (f_des = open("./fifo", O_RDWR))) {
		printf("fail to call open() \n");
		exit(1);
	}

	for (cnt = 0 ; cnt < 3 ; cnt++) {
		if (0 > (nread = read(f_des, msg, MSG_SIZE))) {
			printf("fail to call read() \n");
			exit(1);
		}
		printf("recv: %s \n", msg);
	}
	unlink("./fifo");
	return 0;
}
