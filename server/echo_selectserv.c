#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100
void error_handling(char *buf);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;	
	struct timeval timeout;
	fd_set reads, cpy_reads;

	socklen_t adr_sz;
	int fd_max, str_len, fd_num, i;
	char buf[BUF_SIZE];
	if (2 != argc) {
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	//serv_adr.sin_addr.s_addr = htonl("127.0.0.1");
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (-1 == bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)))
		error_handling("bind() error");
	if (-1 == listen(serv_sock, 5))
		error_handling("listen() error");

	FD_ZERO(&reads);
	FD_SET(serv_sock, &reads);
	fd_max = serv_sock;
	
	printf("fd_max: %d\n", fd_max);
	
	while(1)
	{
		
		cpy_reads = reads;
		
		timeout.tv_sec = 0;
		timeout.tv_usec = 1;

		if (-1 == (fd_num = select(fd_max+1, &cpy_reads, 0, 0, &timeout)))
			break;
		//fd_num = select(fd_max+1, &cpy_reads, 0, 0, &timeout);
		//select(fd_max+1, &cpy_reads, 0, 0, &timeout);
		if (0 == fd_num)
			continue;

		//printf("fd_max: %d, fd_num: %d \n", fd_max, fd_num);		

		for (i = 0 ; i < fd_max+1 ; i++)
		{
			//printf("FD_ISSET: %d = %d\n", i, FD_ISSET(i, &cpy_reads));
			if (FD_ISSET(i, &cpy_reads))
			{
				if (i == serv_sock)
				{
					adr_sz = sizeof(clnt_adr);
					clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
					FD_SET(clnt_sock, &reads);
					if (fd_max < clnt_sock)
						fd_max = clnt_sock;
					printf("connected client : %d \n", clnt_sock);
				}
				else
				{
					str_len = read(i, buf, BUF_SIZE);
					if (0 == str_len)
					{
						FD_CLR(i, &reads);
						close(i);
						printf("closed client : %d \n", i);
					}
					else
					{
						write(i, buf, str_len);
					}
				}
			/*else	// error code
			{
				str_len = read(i, buf, BUF_SIZE);
				if (0 == str_len)
				{
					FD_CLR(i, &reads);
					close(i);
					printf("closed client : %d \n", i);
				}
				else
				{
					write(i, buf, str_len);
				}*/	//
			}
		}
	}
	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	perror(message);
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

