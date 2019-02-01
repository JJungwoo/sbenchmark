#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define TIMEOUT 2
#define BUF_SIZE 4096

void error_handling(char *msg);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock, clnt_len, i, clnt_max;
	unsigned char buf[BUF_SIZE];
	struct sockaddr_in serv_adr, clnt_adr;
	struct timeval tv;
	fd_set fst, tmpfds;

	if (2 != argc) {
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	if (-1 == (serv_sock = socket(AF_INET, SOCK_STREAM, 0)))
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	//serv_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));
	
	if (-1 == bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)))
		error_handling("bind() error");
	
	if (-1 == listen(serv_sock, 5))
		error_handling("listen() error");


/*
	while(1) {	
		FD_ZERO(&fst);
		FD_SET(clnt_sock, &fst);
		print_fd(fst);
		
		if (-1 == (clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_len)))
			error_handling("accept() error");
	
		printf("%d connected!\n", clnt_sock);
		
		if (-1 == select(clnt_sock + 1, &fst, 0, 0, &tv))
			error_handling("select() error");

		close(clnt_sock);
	}
*/

	clnt_max = serv_sock;
	
	FD_ZERO(&fst);
	FD_SET(serv_sock, &fst);

	while (1)
	{
		tmpfds = fst;
		
		//if (-1 == (clnt_max = select(clnt_sock+1, &tmpfds, 0, 0, 0)))
		//if (-1 == (clnt_max = select(clnt_max+1, &tmpfds, NULL, NULL, NULL)))
		//	error_handling("select() error");
		
		if (-1 == select(clnt_max+1, &tmpfds, NULL, NULL, NULL))
			error_handling("select() error");
	
		if (FD_ISSET(serv_sock, &tmpfds))
		{
			clnt_len = sizeof(clnt_adr);
			if (-1 == (clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_len)))
				error_handling("accept() error");
		}
		printf("client socket fd: %d \n", clnt_sock);
		FD_SET(clnt_sock, &fst);
		
		//if (clnt_sock > clnt_max)
		//	clnt_max = clnt_sock;
		for (i = 0 ; i < clnt_max+1; i++)
		{
			puts("accept() overhead..\n");
			if (-1 == (clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_len)))
				error_handling("accept() error");
			FD_SET(clnt_sock, &fst);
			printf("%d connected!\n", clnt_sock);
		}
		/*if (1 == FD_ISSET(clnt_sock, &fst))
		{
			printf("%d ISSET !\n", clnt_sock);
		}*/
		printf("%d ISSET !\n", clnt_sock);
		//print_fd(fst);
		//puts("?\n");
	}


	close(clnt_sock);
	close(serv_sock);
	
	return 0;
}

void print_fd(fd_set fds)
{
	int i;
	for(i=0;i<10;i++)
	{
		printf("%d: %d \n", i, FD_ISSET(i, &fds));
	}
}

void error_handling(char *msg)
{
	perror(msg);
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
