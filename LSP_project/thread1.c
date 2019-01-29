#include <stdio.h>
#include <pthread.h>
void *thread_main(void *arg);

int main(int argc, char *argv[])
{
	pthread_t t_id;
	int thread_param = 5;

	if (0 != pthread_create(&t_id, NULL, thread_main, (void *)&thread_param))
	{
		puts("pthread_create() error!");
		return -1;
	};
	sleep(6); puts("end of main");
	printf("thread_param : %d\n", thread_param);
	return 0;
}

void *thread_main(void *arg)
{
	int i;
	int cnt = *((int *)arg);
	int *t = (int *)arg;
	for (i = 0 ; i < cnt ; i++)
	{
		sleep(1); puts("running thread");
	}
	//cnt+=5;
	*t += 5;
	return NULL;
}
