#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);
void read_routine(int sock, char *buf);
void write_routine(int sock, char *buf);

int main(int argc, char *argv[])
{
	int sock;
	pid_t pid;
	char buf[BUF_SIZE];
	struct sockaddr_in serv_adr;

	if (3 != argc){
		printf("Usage : %s <Ip> <Port> \n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (-1 == sock)
		error_handling("socket() error");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	//serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_adr.sin_port = htons(atoi(argv[2]));
	//serv_adr.sin_port = htons(atoi(argv[1]));

	if (-1 == connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)))
		error_handling("connect() error");
	else 
		puts("connected...");

	pid=fork();

	if (0 == pid)
		write_routine(sock, buf);
	else 
		read_routine(sock, buf);

	close(sock);
	return 0;
}

void read_routine(int sock, char *buf)
{
	while(1)
	{
		int str_len = read(sock, buf, BUF_SIZE);
		if (0 == str_len)
			return;

		buf[str_len] = 0;
		printf("Message from server: %s", buf);
	}
}

void write_routine(int sock, char *buf)
{
	while(1)
	{
		fputs("Input Message(Q to quit): ", stdout);
		fgets(buf, BUF_SIZE, stdin);
		//strcpy(message, "Test\n");
		if (!strcmp(buf, "q\n") || !strcmp(buf, "quit\n"))
		{
			shutdown(sock, SHUT_WR);
			return;
		}
		write(sock, buf, strlen(buf));
	}
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
