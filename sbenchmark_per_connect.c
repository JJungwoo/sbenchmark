#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>

//#define pre_connect 0
//#define JDBG 

#ifdef MUTEX
	#define MUTEX
#else
	#define SPINLOCK
#endif

#define BUF_SIZE 1024
#define MAX_THREAD 100000

void *client_thread(void *data);
void error_handling(char *msg);
long print_time(struct timeval T1, struct timeval T2);

#ifdef MUTEX
pthread_mutex_t mutex;
#else
pthread_spinlock_t spinlock;
#endif

int main(int argc, char *argv[])
{
	int sock, i, str_len, th_count;
	char message[BUF_SIZE];
	pthread_t *cthread[MAX_THREAD];	
	struct timeval T1, T2;
	long time_res = 0, total_t = 0;
	void *thread_res;
	struct sockaddr_in serv_adr;

	pthread_spin_init(&spinlock, 0);
	pthread_mutex_init(&mutex, NULL);

#ifdef JDBG
	printf("argc: %d \n", argc);
	for(i=0;i<argc;i++)
		printf("argv[%d]: %s \n ", i, argv[i]);
#endif
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_adr.sin_port = htons(atoi("3389"));

	th_count = atoi(argv[2]);
	
	gettimeofday(&T1, NULL);

	for (i = 0 ; i < th_count ; i++)
	{
#ifdef JDBG
		printf("%d count th \n", i);
#endif
#ifdef MUTEX
		pthread_mutex_lock(&mutex);
#else
		pthread_spin_lock(&spinlock);
#endif
		if (0 > pthread_create(&cthread[i], NULL, client_thread, (void *)&serv_adr))
			error_handling("thread create error");
#ifdef MUTEX
		pthread_mutex_unlock(&mutex);
#else
		pthread_spin_unlock(&spinlock);
#endif
	}

	for (i = 0 ; i < th_count ; i++)
	{
#ifdef MUTEX
		pthread_mutex_lock(&mutex);
#else
		pthread_spin_lock(&spinlock);
#endif
		if (-1 == pthread_join(cthread[i], &thread_res))
			error_handling("join() error");
#ifdef JDBG
		printf("%d count Thread End \n total: %ld, thread result: %ld \n"
				, i, total_t, (long*)thread_res);
#endif
		total_t += thread_res;

#ifdef MUTEX
		pthread_mutex_unlock(&mutex);
#else
		pthread_spin_unlock(&spinlock);
#endif
	}

	gettimeofday(&T2, NULL);

	time_res = print_time(T1, T2);
	puts("-----------");
#ifdef MUTEX
	puts("mutex");
#else
	puts("spinlock");
#endif
	printf("real time res: %ld total_t: %ld \n", time_res, total_t);
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

#ifdef MUTEX
	pthread_mutex_destroy(&mutex);
#else
	pthread_spin_destroy(&spinlock);
#endif
	return 0;
}

void *client_thread(void *data)
{
	int i, sock;
	long gap; 
	struct sockaddr_in serv_adr;
	struct timespec start_ts, end_ts;

	clock_gettime(CLOCK_REALTIME, &start_ts);

	if (-1 == (sock = socket(PF_INET, SOCK_STREAM, 0)))
		error_handling("socket() error");
	
	serv_adr = *((struct sockaddr_in*)data);
	
	if (-1 == connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)))
		error_handling("connect() error");
#ifdef JDBG
	else
		printf("connected... %d \n", sock);
#endif
	
	close(sock);

	clock_gettime(CLOCK_REALTIME, &end_ts);	

	gap = print_time(start_ts, end_ts);
#ifdef JDBG
	printf("gap: %ld\n", gap);
#endif

	return (void*)gap;
}

long long print_time(struct timespec T1, struct timepec T2)
{
	long sec, usec;
	double time;
	double rate;
	
	time_t t;
	
#ifdef JDBG
	printf("print_time: \n T1: %ld, T2: %ld \n", T1.tv_sec, T2.tv_sec);
#endif

	if (T1.tv_nsec > T2.tv_nsec)
	{
		sec = T2.tv_sec - T1.tv_sec - 1;
		usec = 1000000 + T2.tv_usec - T1.tv_usec;
	}
	else
	{
		sec = T2.tv_sec - T1.tv_sec;
		usec = T2.tv_usec - T1.tv_usec;
	}

	time = (double)sec + (double)usec/1000000;
#ifdef JDBG
	printf("[TIME]: %8ld sec %06ldus. \n time: %f \n", sec, usec, time);
#endif
	return usec;
}


void error_handling(char *msg)
{
	perror(msg);
	exit(1);
}

