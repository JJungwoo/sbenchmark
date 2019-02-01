#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <time.h>
#include "mylist.h"

//#define MUTEX
#define LOOPS 10000000
#define NUM 1000

#ifdef MUTEX
pthread_mutex_t mutex;
#else
pthread_spinlock_t spinlock;
#endif

#define ERROR_CHECK(value, error_code, msg) { if (error_code == value) error_handling(msg); }

#define THREAD_LOOP_CNT 100

void error_handling(char *msg);

void cpujob()
{
    int i;
    double x, step, sum = 0.0;

    step = 1.0/(double) NUM;

    for (i=0; i<NUM; i++) {
        x = (i+0.5) * step;
        sum += 4.0/(1.0 + x*x);
    }
    //printf("PI = %.8f (sum = %.8f)\n", step*sum, sum);
}

#define THREAD_NUM 100
unsigned long long elapse_time[THREAD_NUM]={0,};
void *consumer(void *arg)
{
	unsigned long long elapse_tmp=0;
	//struct timeval tv1, tv2;
	struct timespec start_tv, end_tv;
	int i;
	int thread_idx = (int)arg;
	
	//printf("Consumer TID %lu \n", (unsigned long)gettid());
	
	//while (1)
	for(i=0; i<THREAD_LOOP_CNT; i++)
	{
		ERROR_CHECK(clock_gettime(CLOCK_REALTIME, &start_tv), -1, "clock_gettime() error");
		//GetTimeDiff(0);
		//clock_gettime(CLOCK_REALTIME, &tv1);
#ifdef MUTEX
		pthread_mutex_lock(&mutex);
#else
		pthread_spin_lock(&spinlock);
#endif
		cpujob();
/*
		if (IsEmptyList(mylist))
		{
#ifdef MUTEX
			pthread_mutex_unlock(&mutex);
#else
			pthread_spin_unlock(&spinlock);
#endif
			break;
		}
*/
		//RemoveByNode(mylist, mylist->m_pNodeHead->pNext);	// list front pop

#ifdef MUTEX
		pthread_mutex_unlock(&mutex);
#else
		pthread_spin_unlock(&spinlock);
#endif

		clock_gettime(CLOCK_REALTIME, &end_tv);

		//printf("elapse time %d: %d \n", thread_idx, elapse_time[thread_idx]);

		elapse_tmp = end_tv.tv_sec*1000*1000;
		elapse_tmp += end_tv.tv_nsec/1000;

		elapse_time[thread_idx] += elapse_tmp - (unsigned long long)(start_tv.tv_sec*1000*1000 + start_tv.tv_nsec/1000);

		//elapse_time[thread_idx] += (unsigned long long)(((end_tv.tv_sec - start_tv.tv_sec)*1000*1000) + (end_tv.tv_nsec - start_tv.tv_nsec))/1000;

		//elapse_time[thread_idx] += GetTimeDiff(1);

		//printf("elapse time %d: %llu(us) \n", thread_idx, elapse_time[thread_idx]);
		//printf("elapse_time : 2s: %lld, 2us: %lld, 1s: %lld, 1us: %lld\n", end_tv.tv_sec, end_tv.tv_nsec, start_tv.tv_sec, start_tv.tv_nsec);
	}

		//printf("elapse_time[%d]: %llu(us)\n", thread_idx, elapse_time[thread_idx]);
	return NULL;
}

int main()
{
	MyListC *mylist = NULL; 
	pthread_t thr[THREAD_NUM];
	struct timeval tv1, tv2;
	int i;
	unsigned long long elapse_sum=0;

	memset(elapse_time, 0, sizeof(unsigned long long)*THREAD_NUM);
	memset(thr, 0, sizeof(pthread_t)*THREAD_NUM);

#ifdef MUTEX
	pthread_mutex_init(&mutex, NULL);
#else
	pthread_spin_init(&spinlock, 0);
#endif

	ERROR_CHECK((mylist = (MyListC *)malloc(sizeof(MyListC))), NULL, "malloc() error");

	memset(mylist, 0, sizeof(mylist));	
	constructKListC(mylist);

	for (i = 0 ; i < LOOPS ; i++)
		AddTail(mylist, i);

	printf("Get Count List: %d\n",GetCountList(mylist));

	//clock_gettime(&tv1, NULL);	

	for(i=0; i<THREAD_NUM; i++) {
		//pthread_create(&thr[i], NULL, consumer, &i);
		pthread_create(&thr[i], NULL, consumer, i);
	}

	for(i=0; i<THREAD_NUM; i++) {
		pthread_join(thr[i], NULL);
		//printf("%d: %d \n", i, elapse_time[i]);
		elapse_sum += elapse_time[i];
	}
	//clock_gettime(&tv2, NULL);

	if (tv1.tv_usec > tv2.tv_usec)
	{	
		tv2.tv_sec--;
		tv2.tv_usec += 1000000;
	}

	printf("elapse total: %llu(us)\n", elapse_sum);

	//printf("Result - %ld.%ld \n", tv2.tv_sec - tv1.tv_sec, 
	//		tv2.tv_usec - tv1.tv_usec);

#ifdef MUTEX
	puts("mutex");
	pthread_mutex_destroy(&mutex);
#else
	puts("spinlock");
	pthread_spin_destroy(&spinlock);
#endif
	RemoveAllList(mylist);
	destructKListC(mylist);

	return 0;
}

void error_handling(char *msg)
{
	perror(msg);
	exit(1);
}

