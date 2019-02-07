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
#include <netdb.h>
#include <errno.h>

#define MAX_BUFFER 1024
#define LISTEN_BACKLOG 256
#define MAX_FD_SOCKET 0xff
#define MAX(a, b) a > b ? a : b

int fd_socket[MAX_FD_SOCKET];
int cnt_fd_socket;
fd_set fds_read;
int fd_biggest;
int add_socket(int fd);
int del_socket(int fd);
int mk_fds(fd_set *fds, int *a_fd_socket);\

void multi_child_connecter(void *data);
void error_handling(char *msg);

int main(int argc, char *argv[])
{
	socklen_t len_saddr;
	int fd, fd_listener, ret_recv, ret_select, i;
	char *port, buf[MAX_BUFFER];
	struct addrinfo ai, *ai_ret;
	int rc_gai;
	
	for (i=0; i<MAX_FD_SOCKET; i++)
		fd_socket[i] = -1;
	
	if (2 != argc)
	{
		printf("Usage : %s [port] \n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if (2 == argc)
		port = strdup(argv[1]);
	else 
		port = strdup("0");

	memset(&ai, 0, sizeof(ai));
	ai.ai_family = AF_INET;
	ai.ai_socktype = SOCK_STREAM;
	ai.ai_flags = AI_ADDRCONFIG | AI_PASSIVE;
	if (0 != (rc_gai = getaddrinfo(NULL, port, &ai, &ai_ret))) 
		error_handling("getaddrinfo() error");
	/*{	
		pr_err("Fail: getaddrinfo(): %s", gai_strerror(rc_gai));
		exit(EXIT_FAILURE);
	}*/

	if (-1 == (fd_listener = socket(ai_ret->ai_family, 
			ai_ret->ai_socktype, ai_ret->ai_protocol)))
		error_handling("socket() error");	
	/*{
		pr_err("Fail: socket()");
		exit(EXIT_FAILURE);
	}*/

	if (-1 == bind(fd_listener, ai_ret->ai_addr, ai_ret->ai_addrlen))
		error_handling("bind() error");		
	/*{
		pr_err("Fail: bind()");
		exit(EXIT_FAILURE);
	}*/
	//pr_out("bind: %s", port);
	printf("bind: %s\n",port);
	listen(fd_listener, LISTEN_BACKLOG);
	add_socket(fd_listener);
	while(1)
	{
		fd_biggest = mk_fds(&fds_read, fd_socket);
		if (-1 == (ret_select = select(fd_biggest+1, &fds_read, NULL, NULL, NULL))) 
			error_handling("select() error");
		/*{
			pr_err("Fail: select()");
			exit(EXIT_FAILURE);
		}*/
		//pr_out("\tselect = (%d)", ret_select);
		printf("\tselect = (%d)\n", ret_select);
		if (FD_ISSET(fd_listener, &fds_read)) {
			struct sockaddr_storage saddr_c;
			len_saddr = sizeof(saddr_c);
			fd = accept(fd_listener, (struct sockaddr *)&saddr_c, &len_saddr);
			if (-1 == fd)
				error_handling("accept() error");
			/*{
				pr_err("Error get connection from listen socket");
				continue;
			}*/
			if (-1 == add_socket(fd))
				error_handling("add_socket() error");
			/*{
				pr_err("Fail: add_socket()");
				exit(EXIT_FAILURE);
			}*/
			//pr_out("accept: add socket (%d)", fd);
			printf("accept: add socket (%d)\n", fd);
			continue;
		}
		for (i=1; i<cnt_fd_socket; i++) {
			if (FD_ISSET(fd_socket[i], &fds_read)) {
				//pr_out("FD_ISSET: normal-inband");
				printf("FD_ISSET: normal-inband");
				if (-1 == (ret_recv = recv(fd_socket[i], buf, sizeof(buf), 0))) {
					//pr_err("fd(%d) recv() error(%s)", fd_socket[i], strerror(errno));
					printf("fd(%d) recv() error(%s)\n", fd_socket[i], strerror(errno));
				} else {
					if (0 == ret_recv) {
						//pr_out("fd(%d): Session closed", fd_socket[i]);
						printf("fd(%d): Session closed\n", fd_socket[i]);
						del_socket(fd_socket[i]);
					} else {
						//pr_out("recv(fd=%d, n=%d) = %.*s", fd_socket[i], ret_recv, ret_recv, buf);
						printf("recv(fd=%d, n=%d) = %.*s", fd_socket[i], ret_recv, ret_recv, buf);
					}
				}
			}
		} /* loop: for(i) */
	} /* loop: while */
	return 0;
}

int add_socket(int fd)
{
	if (MAX_FD_SOCKET > cnt_fd_socket) {
		fd_socket[cnt_fd_socket] = fd;
		return ++cnt_fd_socket;
	} else {
		return -1;
	}
}

int del_socket(int fd)
{
	int i, flag;
	flag = 0; /* 1: found, 0: not found */
	close(fd);
	for (i=0; i<cnt_fd_socket; i++) { 
		if (fd_socket[i] == fd) {
			if (i != (cnt_fd_socket-1)) 
				fd_socket[i] = fd_socket[cnt_fd_socket-1];
			fd_socket[cnt_fd_socket-1] = -1;
			flag = 1;
			break;
		}
	}
	if (0 == flag)
		return -1;
	--cnt_fd_socket;
	return i;
}

int mk_fds(fd_set *fds, int *a_fd_socket)
{
	int i, fd_max;
	FD_ZERO(fds);
	for (i=0, fd_max=-1; i<cnt_fd_socket; i++) {
		fd_max = MAX(fd_max, a_fd_socket[i]);
		FD_SET(a_fd_socket[i], fds);
	}
	return fd_max; /* fd max number return */
}

void multi_child_connecter(void *data)
{

}

void error_handling(char *msg)
{
	perror(msg);
	exit(1);
}
