#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include "jlib.h"

//#define pre_connect 0
//#define JDBG 

#ifdef MUTEX
	#define MUTEX
#else
	#define SPINLOCK
#endif

#define BIG_BUF_SIZE 1048576 /* max 1024K(1MB) */
#define BUF_SIZE 4096 /* max 4k */
#define MAX_THREAD 100000
#define KSIZE 16

#define MESSAGE "AAAAAAAAAABBBBBBBBBBCCCCCCCCCCDDDDDDDDDD" // 40B + '\0' = 41B
//#define OK_MSG "AAAAAAAAAABBBBBBBBBBCCCCCCCCCCDDDDDDDDDDEEEEEEEEEEFFFFFFFFFFGGGG" // 64B
#define OK_MSG "AAAAAAAAAABBBBBBBBBBCCCCCCCCCCDDDDDDDDDDEEEEEEEEEE\ 
AAAAAAAAAABBBBBBBBBBCCCCCCCCCCDDDDDDDDDDEEEEEEEEEE\ 
AAAAAAAAAABBBBBBBBBBCCCCCCCCCCDDDDDDDDDDEEEEEEEEEE\ 
AAAAAAAAAABBBBBBBBBBCCCCCCCCCCDDDDDDDDDDEEEEEEEEEE\ 
AAAAAAAAAABBBBBBBBBBCCCCCCCCCCDDDDDDDDDDEEEEEEEEEE\
AAAAAAAAAABBBBBBBBBBCCCCCCCCCCDDDDDDDDDDEEEEEEEEEE\
AAAAAAAAAABBBBBBBBBBCCCCCCCCCCDDDDDDDDDDEEEEEEEEEE\
AAAAAAAAAABBBBBBBBBBCCCCCCCCCCDDDDDDDDDDEEEEEEEEEE\
AAAAAAAAAABBBBBBBBBBCCCCCCCCCCDDDDDDDDDDEEEEEEEEEE\
AAAAAAAAAABBBBBBBBBBCCCCCCCCCCDDDDDDDDDDEEEEEEEEEE"
#define EXIT_MESSAGE "exit"

void *client_thread(void *data);
void error_handling(char *msg);
long long print_time(struct timespec T1, struct timespec T2);

int file_size;

typedef struct ret_thread 
{
	unsigned long long per_time;
	int checksum;
}ret_t;

#ifdef MUTEX
pthread_mutex_t mutex;
#else
pthread_spinlock_t spinlock;
#endif

int main(int argc, char *argv[])
{
	int sock, i, str_len, th_count, checksum = 0;
	char message[BUF_SIZE];
	pthread_t *cthread[MAX_THREAD];	
	struct timespec T1, T2;
	unsigned long long time_res = 0, total_t = 0;
	void *thread_res;
	//struct ret_thread ret_t[MAX_THREAD];
	struct sockaddr_in serv_adr;
	
#ifdef MUTEX
	pthread_mutex_init(&mutex, NULL);
#else
	pthread_spin_init(&spinlock, 0);
#endif

#ifdef JDBG
	if (1 < argc)
	{
		printf("argc: %d \n", argc);
		for(i=0;i<argc;i++)
			printf("argv[%d]: %s\n", i, argv[i]);
	} else {
		pr_err("argc error");
		exit(EXIT_FAILURE);
	}
#endif
	
	
	if (1 < argc && !strcmp(argv[1], "--h"))
	{
		puts("-----------------------------------");
		puts("sbenchmark is server test benchmark");
		puts("./sbenchmark <ip> <port> -t 5 ");
		puts("-----------------------------------");
		exit(EXIT_SUCCESS);
	} 
	
	if (4 > argc) {
		pr_err("\n--------------------------------------\n input value is bad command line format \n \
sbenchmark help command is \"--h\"\n--------------------------------------");
		exit(EXIT_FAILURE);
	}
	else
	{
		if (-1 == (sock = socket(PF_INET, SOCK_STREAM, 0)))
			error_handling("socket() error");
	
		memset(&serv_adr, 0, sizeof(serv_adr));
		serv_adr.sin_family = AF_INET;
		serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
		serv_adr.sin_port = htons(atoi(argv[2]));

		if (!strcmp("-t", argv[3]))
		{	
			if (0 < argv[4])	// thread counter error check
			{
				if (!strcmp("-f", argv[5]))
					if (0 < argv[6])
						file_size=argv[6];	// 1 (Per/Kilobyte)	
								
				th_count = atoi(argv[4]);
				
				if (0 != clock_gettime(CLOCK_REALTIME, &T1))
				{
					pr_err("clock_gettime() error");
					exit(EXIT_FAILURE);
				}

				for (i = 0 ; i < th_count ; i++)
				{
#ifdef JDBG
					printf("%d count th \n", i);
#endif
					if (0 > pthread_create(&cthread[i], 0, client_thread, (void *)&serv_adr))
						error_handling("thread create error");
				}

#ifdef MUTEX
				pthread_mutex_lock(&mutex);
#else
				pthread_spin_lock(&spinlock);
#endif
				for (i = 0 ; i < th_count ; i++)
				{
					if (-1 == pthread_join(cthread[i], &thread_res))
						error_handling("join() error");
#ifdef JDBG
					else 
						pr_out("counter: %d\n", i);	
					printf("%d count Thread End \n total: %ld, thread result: %ld \n"
							, i, total_t, (unsigned long long*)thread_res);	
					checksum++;
#endif
					total_t += (unsigned long long)thread_res;
					//printf("%llu, %d\n", thread_res[i]->elapsed_time, thread_res[i]->checksum);
				}
#ifdef MUTEX
				pthread_mutex_unlock(&mutex);
#else
				pthread_spin_unlock(&spinlock);
#endif

				if (0 != clock_gettime(CLOCK_REALTIME, &T2))
				{
					pr_err("clock_gettime() error");
					exit(EXIT_FAILURE);
				}
			}
			else
				error_handling("thread counter error");
		
#if pre_connect
			if (-1 == connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)))
#endif
		}

	}
	
	time_res = print_time(T1, T2);
	puts("(result)-----------------");
	//printf("real time res: %ld total_t: %ld \n", time_res, total_t);
	pr_out("Real Time: %llu(us) total: %llu(us)", time_res, total_t);

#ifdef JDBG
	printf("checksum: %d\n", checksum);
#endif

#ifdef MUTEX
	puts("mutex");
	pthread_mutex_destroy(&mutex);
#else
	puts("spinlock");
	pthread_spin_destroy(&spinlock);
#endif
	return 0;
}

void *client_thread(void *data)
{
	int i, sock, send_len, recv_len;
	long long gap; 
	struct sockaddr_in serv_adr;
	struct timespec start_ts, end_ts;
	char send_msg[BUF_SIZE], recv_msg[BUF_SIZE];
	//struct ret_thread ret_t;

	//memset(&ret_t, 0, sizeof(ret_thread));
	memcpy(send_msg, OK_MSG, strlen(OK_MSG)+1);

	clock_gettime(CLOCK_REALTIME, &start_ts);

	if (-1 == (sock = socket(PF_INET, SOCK_STREAM, 0)))
		error_handling("socket() error");
	
	serv_adr = *((struct sockaddr_in*)data);
	
	if (-1 == connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)))
		error_handling("connect() error");
#ifdef JDBG
	else
		printf("(%d) socket connected... \n", sock);
#endif
	
	//for (i=0; i<file_size*KSIZE ; i++)
	//{
		if (-1 == (send_len = send(sock, send_msg, BUF_SIZE, 0)))
			error_handling("send() error");
	//}
	if (-1 == (recv_len = recv(sock, recv_msg, BUF_SIZE, 0)))
		error_handling("recv() error");
	if (!strcmp("success",recv_msg))
	{
		//ret_t.checksum = 1;
		send_len = 0;
		memset(send_msg, 0, BUF_SIZE);
		memcpy(send_msg, EXIT_MESSAGE, BUF_SIZE);
		if (-1 == (send_len = send(sock, send_msg, BUF_SIZE, 0)))
			error_handling("send() error");
		close(sock);
	}
#ifdef JDBG
	pr_out("recv (fd=%d,n=%d) = %.*s",
			sock, recv_len, recv_len, recv_msg); 
#endif

	clock_gettime(CLOCK_REALTIME, &end_ts);	

	gap = print_time(start_ts, end_ts);
	//ret_t.elapsed_time = gap;
#ifdef JDBG
	printf("gap: %ld\n", gap);
#endif

	return (void*)gap;
	//return (void *)ret_t;
}

long long print_time(struct timespec T1, struct timespec T2)
{
	long long sec, nsec, time, usec;
	
#ifdef JDBG
	printf("print_time: \n T1: %ld, T2: %ld \n", T1.tv_sec, T2.tv_sec);
#endif

	if (T1.tv_nsec > T2.tv_nsec)
	{
		sec = T2.tv_sec - T1.tv_sec - 1;
		nsec = 1000000000 + T2.tv_nsec - T1.tv_nsec;
	}
	else
	{
		sec = T2.tv_sec - T1.tv_sec;
		nsec = T2.tv_nsec - T1.tv_nsec;
	}

	usec = sec*1000*1000 + nsec/1000;
#ifdef JDBG
	//printf("[Elapsed Time]: %8ld(sec) %06ld(us). \n total time: %8ld(sec)\n", sec, nsec, time);
	pr_out("[Elapsed Time]: %lld(sec) %lld(nano) total usec: %lld(us)", sec, nsec, usec);
#endif
	return usec;
}


void error_handling(char *msg)
{
	//perror(msg);
	pr_err(msg);
	exit(EXIT_FAILURE);
}

