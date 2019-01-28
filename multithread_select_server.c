#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

#define MAX_BUFFER 1024

void multi_child_connecter(void *data);
void error_handling(char *msg);

int main(int argc, char *argv[])
{
	int server_sockfd, client_sockfd;
	int state, client_len;
	
	struct sockaddr_in clnt_adr, serv_adr;

	char buf[MAX_BUFFER];
	
	if (2 != argc)
	{
		printf("Usage : %s [port] \n", argv[0]);
		exit(1);
	}

		




	return 0;
}

void multi_child_connecter(void *data)
{

}

void error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
