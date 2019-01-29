#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define TIMEOUT 2
#define BUF_SIZE 4096
#define EPOLL_SIZE 4096
void error_handling(char *msg);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock, clnt_len, i;
	unsigned char buf[BUF_SIZE];
	struct sockaddr_in serv_adr, clnt_adr;
	struct timeval tv;
	fd_set fst;

	struct epoll_event *ep_events;
	struct epoll_event event;
	int epfd, event_cnt;

	if (2 != argc) {
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	if (-1 == (serv_sock = socket(PF_INET, SOCK_STREAM, 0)))
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_adr.sin_port = htons(atoi("3389"));
	
	if (-1 == bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)))
		error_handling("bind() error");
	
	if (-1 == listen(serv_sock, 5))
		error_handling("listen() error");

	clnt_len = sizeof(clnt_adr);
	
	epfd = epoll_create(EPOLL_SIZE);
	ep_events = malloc(sizeof(struct epoll_event)*EPOLL_SIZE);

	event.events = EPOLLIN;
	event.data.fd = serv_sock;
	epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);

	while(1) {	
		if (-1 == (event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1)))
			error_handling("epoll_wait() error");
		
		for (i = 0 ; i < event_cnt ; i++)
		{
			if (ep_events[i].data.fd == serv_sock)
			{
				if (-1 == (clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_len)))
					error_handling("accept() error");
				event.events = EPOLLIN;
				event.data.fd = clnt_sock;
				epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
				printf("connected client: %d\n", clnt_sock);
			}	
		}
	}

	close(clnt_sock);
	close(serv_sock);
	close(epfd);
	
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
