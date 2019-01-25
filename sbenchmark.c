#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>

#define pre_connect

#define BUF_SIZE 1024
void error_handling(char *msg);

int main(int argc, char *argv[])
{
	int sock, i, str_len;
	char message[BUF_SIZE];
	struct sockaddr_in serv_adr;
	
	printf("argc: %d \n", argc);
	for(i=0;i<argc;i++)
		printf("argv[%d]: %s \n ", i, argv[i]);
		
	if (1 > argc) {
		printf("input value is bad command line format \n \
sbenchmark help command is \"--h\" \n");
		exit(1);
	}
	
	if (!strcmp(argv[1], "--h"))
	{
		puts("nono");
		puts("sbenchmark is server test benchmark");
		puts("./sbenchmark <ip> <port> -t 5 ");
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
			if (0 < argv[4])
			{
								

			}
		
#if pre_connect
			if (-1 == connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)))
#endif
		}


	}

	




	return 0;
}


void error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

