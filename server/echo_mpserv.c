#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);
void read_childproc(int sig);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;

	pid_t pid;
	struct sigaction act;
	socklen_t adr_sz;
	int str_len, state;
	char buf[BUF_SIZE];
	if (2 != argc) {
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}	

	/* sigaction struct init */
	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	state = sigaction(SIGCHLD, &act, 0);
	
	/* socket struct init  */
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));

	if (-1 == state)
		error_handling("sigaction() error");
	if (-1 == bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)))
		error_handling("bind() error");
	if (-1 == listen(serv_sock, 5))
		error_handling("listen() error");

	puts("Start Echo_mpServer");
	while(1)
	{
		puts("!!");
		adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
		if (-1 == clnt_sock)
		{
			perror("accept() error!");
			continue;
		}
		else 
			puts("new client connected....");

		pid = fork();
		if (-1 == pid)
		{
			close(clnt_sock);
			continue;
		}
		if (0 == pid)
		{
			close(serv_sock);
			while(0 != (str_len = read(clnt_sock, buf, BUF_SIZE)))
				write(clnt_sock, buf, str_len);

			close(clnt_sock);
			puts("client disconnected...");
			return 0;
		} else close(clnt_sock);
	}
	close(serv_sock);
	return 0;
}	

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}	

void read_childproc(int sig)
{
	pid_t pid;
	int status;
	pid = waitpid(-1, &status, WNOHANG);
	printf("removed proc id : %d \n",pid);
}




