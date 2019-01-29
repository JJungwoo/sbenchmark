#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define MSG_SIZE 80

int main()
{
    char msg[MSG_SIZE];
    int filedes;
    int cnt;

    // open fifo file
    if((filedes = open("./fifo", O_WRONLY)) < 0){
        printf("fail to call open()\n");
        exit(1);
    }

    for(cnt=0; cnt<3; cnt++){
        printf("input a message : ");
        gets(msg);
        msg[MSG_SIZE-1] = '\0';
        if(write(filedes, msg, MSG_SIZE)==-1){
            printf("fail to call write()\n");
            exit(1);
        }
        sleep(1);
    }
	return 0;
}

