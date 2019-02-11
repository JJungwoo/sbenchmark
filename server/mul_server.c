#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define MAXLINE 1024

void *myFunc(void *arg);

int main(){

	
	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket error");
		return 1;
	}

	memset((void *)&sockaddr,0x00,sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sockaddr.sin_port = htons(atoi(argv[1]));

	if( bind(listenfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) == -1)
	{
		perror("bind error");
		return 1;
	}

	if(listen(listenfd, 5) == -1)
	{
		perror("listen error");
		return 1;
	}

	while(1)
	{
		sockfd = accept(listenfd, (struct sockaddr *)&sockaddr, &socklen);
		if(sockfd == -1)
		{
			perror("accept error");
			return 1;
		}
		th_id = pthread_create(&thread_t, NULL, myFunc, (void *)&sockfd);
		if(th_id != 0)
		{
			perror("Thread Create Error");
			return 1;
		}
		pthread_detach(thread_t);
	}
}

void *myFunc(void *arg)
{
	int sockfd;
	int readn, writen;
	char buf[MAXLINE];
	sockfd = *((int *)arg);

	while(1)
	{
		readn = read(sockfd, buf, MAXLINE);
		if(readn <= 0)
		{
			perror("Read Error");
			return NULL;
		}
		writen = write(sockfd, buf, readn);
		if(readn != writen)
		{
			printf("write error %d : %d\n", readn, writen);
			return NULL;
		}
	}
}
