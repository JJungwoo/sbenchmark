#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>

#define pre_connect 0

#define BUF_SIZE 4096
#define MAX_THREAD 100000
struct timespec diff_timespec(struct timespec start, struct timespec end);
long long nanosec_elapsed(struct timespec diff);
void *client_thread(void *data);
void error_handling(char *msg);

int main(int argc, char *argv[])
{
	int sock[MAX_THREAD], i, str_len, status, th_count;
	char message[BUF_SIZE];
	pthread_t *cthread[MAX_THREAD];	
	struct sockaddr_in serv_adr;
	struct timespec start, end, diff;

	/* time clock */

	clock_t start_t, end_t;
	float res;

	///////////////

	th_count = atoi(argv[2]);
	printf("argc: %d \n", argc);
	for(i=0;i<argc;i++)
		printf("argv[%d]: %s \n ", i, argv[i]);

	for (i = 0 ; i < th_count ; i++)
	{
		if (-1 == (sock[i] = socket(PF_INET, SOCK_STREAM, 0)))
			error_handling("socket() error");
	}
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_adr.sin_port = htons(atoi(argv[1]));
	
	clock_gettime(CLOCK_MONOTONIC, &start);
	start_t = clock();

	for (i = 0 ; i < th_count ; i++)
	{
		if (-1 == connect(sock[i], (struct sockaddr*)&serv_adr, sizeof(serv_adr)))
			error_handling("connect() error");
		else
			printf("connected... %d -> %d \n", i, sock[i]);
	}

	for (i = 0 ; i < th_count ; i++)
	{
		close(sock[i]);
	}
	
	end_t = clock();
	res = (float)(end_t - start_t)/CLOCKS_PER_SEC;
	clock_gettime(CLOCK_MONOTONIC, &end);
	diff = diff_timespec(start, end);
	//printf("%lld nanoseconds \n", nanosec_elapsed(diff)/100000000);
	printf("time clock start: %d, end: %d, result: %.3f \n", start, end, res);
	printf("%lld nanoseconds \n", (double)nanosec_elapsed(diff));
	
/*
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
*/
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

struct timespec diff_timespec(struct timespec start, struct timespec end)
{
	struct timespec result;
		
	if (end.tv_nsec < start.tv_nsec) {
		result.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
		result.tv_sec = end.tv_sec - 1 - start.tv_sec;
	}
	else
	{
		result.tv_nsec = end.tv_nsec - start.tv_nsec;
		result.tv_sec = end.tv_sec - start.tv_sec;
	}

	return result;
}

long long nanosec_elapsed(struct timespec diff) 
{
	return ((long long)diff.tv_sec * 1000000000) + diff.tv_nsec;
}

void *client_thread(void *data)
{
	int i;
	int a = *(int *)data;

	//printf("client_thread number: %d \n", a);

	return (void *)(i * 100);
}

void error_handling(char *msg)
{
	perror(msg);
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

