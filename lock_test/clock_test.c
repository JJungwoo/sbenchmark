#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/syscall.h>	/* For SYS_xxx definitions */
#include <sys/types.h>		/* For pid_t */

//#define THREAD_NUM 10
#define LOOP 100000

#define ERROR_CHECK(value, msg) { if (-1 == value) error_handling(msg); }

void error_handling(char *msg);

static inline pid_t gettid (void)
{
	return (pid_t) syscall (SYS_gettid);	/* or __NR_gettid */
}

void clock_per_job()
{
	unsigned long long elapse_time = 0;
	unsigned long long elapse_time_gtod = 0;
	int i;
	struct timespec s_ts, e_ts;	// thread-safe func
	struct timeval s_tv, e_tv;	// not thread-safe func

	ERROR_CHECK(clock_gettime(CLOCK_REALTIME, &s_ts),"clock_gettime() error");
	ERROR_CHECK(gettimeofday(&s_tv, NULL), "gettimeofday() error");
	
	for (i=0 ;i<LOOP ;i++)
		;
	
	ERROR_CHECK(clock_gettime(CLOCK_REALTIME, &e_ts),"clock_gettime() error");
	ERROR_CHECK(gettimeofday(&e_tv, NULL), "gettimeofday() error");

	elapse_time = (((e_ts.tv_sec - s_ts.tv_sec)*1000*1000) + 
					((e_ts.tv_nsec - s_ts.tv_nsec)/1000));
	elapse_time_gtod = (((e_tv.tv_sec - s_tv.tv_sec)*1000*1000) + 
					((e_tv.tv_usec - s_tv.tv_usec)));
#ifdef JDBG
	printf("TID: %lu = elapse time: %llu(us) \n", (unsigned long)gettid(), elapse_time);
	printf("(gtod) TID: %lu = elapse time: %llu(us) \n", (unsigned long)gettid(), elapse_time_gtod);
#endif

}

int main()
{
	struct timespec s_ts, e_ts;
	struct timeval s_tv, e_tv;
	unsigned long long elapse_time = 0;
	int i = 0;
	pthread_t *cthreads[THREAD_NUM];

	memset(cthreads, 0, sizeof(pthread_t)*THREAD_NUM);

	ERROR_CHECK(clock_gettime(CLOCK_REALTIME, &s_ts),"clock_gettime() error");
	
	for (i=0; i<THREAD_NUM; i++)
	{
		pthread_create(&cthreads[i], NULL, clock_per_job, NULL);
	}

	for (i=0; i<THREAD_NUM; i++)
	{
		pthread_join(cthreads[i], NULL);
	}

	ERROR_CHECK(clock_gettime(CLOCK_REALTIME, &e_ts),"clock_gettime() error");

	elapse_time = (((e_ts.tv_sec - s_ts.tv_sec)*1000*1000) + 
					((e_ts.tv_nsec - s_ts.tv_nsec)/1000));
#ifdef JDBG	
	printf("start sec: %llu, nsec: %llu, end sec: %llu, nsec: %llu, main elapse_time: %llu(us) \n",	s_ts.tv_sec, s_ts.tv_nsec, e_ts.tv_sec, e_ts.tv_nsec, elapse_time);
#endif
	return 0;
}

void error_handling(char *msg)
{
	perror(msg);
	exit(1);
}
