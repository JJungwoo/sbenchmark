#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>

#define pre_connect 0
//#define JDBG 

#define MTX_ON 0

#if MTX_ON
	#define MUTEX
#else
	#define SPINLOCK
#endif

#define BUF_SIZE 1024
#define MAX_THREAD 100000


void *client_thread(void *data);
void error_handling(char *msg);
long print_time(struct timeval T1, struct timeval T2);

pthread_spinlock_t spinlock;
pthread_mutex_t mutex;

int count;

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
	
	th_count = atoi(argv[2]);
	
	gettimeofday(&T1, NULL);


	for (i = 0 ; i < th_count ; i++)
	{
#ifdef JDBG
		printf("%d count th \n", i);
#endif
		if (0 > pthread_create(&cthread[i], NULL, client_thread, NULL))
			error_handling("thread create error");
	}
	while(count != th_count)
		printf("!!!! %d \n", count);

/*
	for (i = 0 ; i < th_count ; i++)
	{
		if (-1 == pthread_join(cthread[i], &thread_res))
			error_handling("join() error");
#ifdef JDBG
		printf("%d count Thread End \n total: %ld, thread result: %ld \n"
				, i, total_t, (long*)thread_res);
#endif
		total_t += thread_res;

	}
*/

	gettimeofday(&T2, NULL);

	time_res = print_time(T1, T2);
	puts("-----------");
#ifdef MUTEX
	puts("mutex");
#else
	puts("spinlock");
#endif	
	printf("real time res: %ld total_t: %ld \n", time_res, total_t);
	printf("count : %d \n", count);

	pthread_spin_destroy(&spinlock);
	pthread_mutex_destroy(&mutex);

	return 0;
}

void *client_thread(void *data)
{
#ifdef MUTEX
	pthread_mutex_lock(&mutex);
#else
	pthread_spin_lock(&spinlock);
#endif	
	count += 1;	
#ifdef MUTEX
	pthread_mutex_unlock(&mutex);
#else
	pthread_spin_unlock(&spinlock);
#endif

	return (void*) 0;
}

long print_time(struct timeval T1, struct timeval T2)
{
	long sec, usec;
	double time;
	double rate;
	
	time_t t;
	
#ifdef JDBG
	printf("print_time: \n T1: %ld, T2: %ld \n", T1.tv_sec, T2.tv_sec);
#endif

	if (T1.tv_usec > T2.tv_usec)
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

