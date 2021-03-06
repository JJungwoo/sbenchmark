#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define MAX_THREAD 100

void * thread_inc(void *arg);
void * thread_des(void *arg);
long long num = 0;
pthread_mutex_t mutex;

int main(int argv, char *argc[])
{
	pthread_t thread_id[MAX_THREAD];
	int i;
	
	pthread_mutex_init(&mutex, NULL);

	printf("sizeof long long: %d \n", sizeof(long long));
	for (i = 0 ; i < MAX_THREAD ; i++)
	{
		if (i%2)	
			pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
		else
			pthread_create(&(thread_id[i]), NULL, thread_des, NULL);
	}

	for (i = 0 ; i < MAX_THREAD ; i++)
		pthread_join(thread_id[i], NULL);

	printf("result : %lld \n", num);
	pthread_mutex_destroy(&mutex);
	return 0;
}

void *thread_inc(void *arg)
{
	int i;
	pthread_mutex_lock(&mutex);
	for (i = 0 ; i < 50000000 ; i++)
		num += 1;
	pthread_mutex_unlock(&mutex);
	return NULL;
}

void *thread_des(void *arg)
{
	int i;
	pthread_mutex_lock(&mutex);
	for (i = 0 ; i < 50000000 ; i ++)
	{
		num -= 1;
	}
	pthread_mutex_unlock(&mutex);
	return NULL;
}
