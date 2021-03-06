#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024
#define SHM_KEY (key_t)0111

int main()
{
	int shmid, len;
	void *shmaddr;

	if (-1 == (shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT|0666)))
	{
		perror("shmget failed");
		exit(1);		
	}

	if ((void *)-1 == (shmaddr = shmat(shmid, NULL, 0)))
	{
		perror("shmat failed");
		exit(1);
	}

	printf("recived from shared memory: %s\n", (char *)shmaddr);

	if (-1 == shmdt(shmaddr))
	{
		perror("shmdt failed");
		exit(1);
	}
	if (-1 == shmctl(shmid, IPC_RMID, 0))
	{
		perror("shmctl failed");
		exit(1);
	}

	return 0;
}
