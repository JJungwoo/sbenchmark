#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>

#define pre_connect 0

#define BUF_SIZE 1024
#define MAX_THREAD 100
void *client_thread(void *data);
void error_handling(char *msg);

int main(int argc, char *argv[])
{
	int sock, i, str_len, status, th_count;
	char message[BUF_SIZE];
	pthread_t *cthread[MAX_THREAD];	

	printf("argc: %d \n", argc);
	for(i=0;i<argc;i++)
		printf("argv[%d]: %s \n ", i, argv[i]);


	th_count = atoi(argv[2]);

	for (i = 0 ; i < th_count ; i++)
	{
		//puts("@@");
		printf("%d count th \n", i);
		if (0 > pthread_create(&cthread[i], NULL, client_thread, (void *)&i))
			error_handling("thread create error");
	}

	for (i = 0 ; i < th_count ; i++)
	{
		pthread_join(cthread[i], (void **)&status);
		//printf("%d count Thread End: %d \n", i, status);
	}

	/*
	if (!strcmp(argv[1], "--h"))
	{
		puts("nono");
		puts("sbenchmark is server test benchmark");
		puts("./sbenchmark <ip> <port> -t 5 ");
	} 

	if (5 > argc) {
		error_handling("input value is bad command line format \n \
sbenchmark help command is \"--h\" \n");
		exit(1);
	}
	else
	{
		if (-1 == (sock = socket(PF_INET, SOCK_STREAM, 0)))
			error_handling("socket() error");
	
		memset(&serv_adr, 0, sizeof(serv_adr));
		serv_adr.sin_family = AF_INET;
		serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
		serv_adr.sin_port = htons(atoi(argv[2]));

		if (!strcmp(argv[3], "-p"))
		{	
			if (0 < argv[4])	// thread counter error check
			{
								
			}
			else
				error_handling("thread counter error");
		
#if pre_connect
			if (-1 == connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)))
#endif
		}

	}
	*/

	return 0;
}

void *client_thread(void *data)
{
	int i, sock;
	int a = *(int *)data;
	struct sockaddr_in serv_adr;
	
	if (-1 == (sock = socket(PF_INET, SOCK_STREAM, 0)))
		error_handling("socket() error");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//serv_adr.sin_port = htons(atoi(argv[1]));
	serv_adr.sin_port = htons(atoi("3389"));
	
	if (-1 == connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)))
		error_handling("connect() error");
	else
		printf("connected... %d \n", sock);

	printf("client_thread number: %d \n", a);

	close(sock);
	return (void *)(i * 100);
}

void error_handling(char *msg)
{
	perror(msg);
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

