/* vim: set ts=4 sw=4: */
/* Filename    : io_select.c
 * Description : I/O multiplexing implementation with select()
 * Author      : SunYoung Kim <sunyzero@yahoo.com>
 * Notes       : 
 */
//#include "stdalsp.h"
#include "jlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

#define LISTEN_BACKLOG	256
#define MAX_FD_SOCKET	0xff
#define MAX(a, b)	a > b ? a : b

#define BUF_SIZE 4096
#define SMESSAGE "success"

int		fd_socket[MAX_FD_SOCKET];
int		cnt_fd_socket;

/* I/O multiplexing var */
fd_set	fds_read;
int		fd_biggest;

int add_socket(int fd);
int del_socket(int fd);
int mk_fds(fd_set *fds, int *a_fd_socket);

int main(int argc, char *argv[])
{
	/* network var			*/
	socklen_t	len_saddr;
	int		fd, fd_listener;
	int		ret_recv, send_len;
	char	*port, buf[BUF_SIZE], send_msg[BUF_SIZE];
	/* I/O multiplexing var */
	int		ret_select;
	/* logical var 			*/
	int		i;

	for (i=0; i<MAX_FD_SOCKET; i++) {
		fd_socket[i] = -1;
	}

	if (argc != 2) {
		printf("%s [port number]\n", argv[0]);
	}
	if (argc == 2) {
		port = strdup(argv[1]);
	} else {
		port = strdup("0");	/* random port */
	}
	memcpy(send_msg, SMESSAGE, strlen(SMESSAGE)+1);

	struct addrinfo ai, *ai_ret;
	int     rc_gai;
	memset(&ai, 0, sizeof(ai));
	ai.ai_family = AF_INET;
	ai.ai_socktype = SOCK_STREAM;
	ai.ai_flags = AI_ADDRCONFIG | AI_PASSIVE;

	if ((rc_gai = getaddrinfo(NULL, port, &ai, &ai_ret)) != 0) {
		pr_err("Fail: getaddrinfo():%s", gai_strerror(rc_gai));
		exit(EXIT_FAILURE);
	}

	if ((fd_listener = socket(
					ai_ret->ai_family,
					ai_ret->ai_socktype,
					ai_ret->ai_protocol)) == -1) {
		pr_err("Fail: socket()");
		exit(EXIT_FAILURE);
	}

	if (bind(fd_listener, ai_ret->ai_addr, ai_ret->ai_addrlen) == -1) {
		pr_err("Fail: bind()");
		exit(EXIT_FAILURE);
	}

	if (!strncmp(port, "0", strlen(port))) { /* port 번호를 알아낸다. */
		struct sockaddr_storage saddr_s;
		len_saddr = sizeof(saddr_s);
		getsockname(fd_listener, (struct sockaddr *)&saddr_s, &len_saddr);
		if (saddr_s.ss_family == AF_INET) {
			pr_out("bind : IPv4 Port : #%d",
					ntohs(((struct sockaddr_in *)&saddr_s)->sin_port));
		} else  if (saddr_s.ss_family == AF_INET6) {
			pr_out("bind : IPv6 Port : #%d",
					ntohs(((struct sockaddr_in6 *)&saddr_s)->sin6_port));
		} else {
			pr_out("getsockname : ss_family=%d", saddr_s.ss_family);
		}
	} else {
		pr_out("bind : %s", port);
	}
	listen(fd_listener, LISTEN_BACKLOG);
	add_socket(fd_listener);	/* 감시할 소켓 리스트에 리스너 소켓 등록 */

	while (1) {
		/* waiting for data (no timeout) */
		fd_biggest = mk_fds(&fds_read, fd_socket);
		if ((ret_select = select(fd_biggest+1, &fds_read, NULL, NULL, NULL)) == -1) {
			/* error */
		}
		pr_out("\tselect = (%d)", ret_select);

		/* IS New connection ? */
		if (FD_ISSET(fd_listener, &fds_read)) {
			struct sockaddr_storage saddr_c;

			/* Accept only one connection allowed per cycle */
			len_saddr = sizeof(saddr_c);
			fd = accept(fd_listener, (struct sockaddr *)&saddr_c, &len_saddr);
			if (fd == -1) {
				pr_err("Error get connection from listen socket");
				continue;
			}
			if (add_socket(fd) == -1) {
				/* error : force to disconnect. */
			}
			pr_out("accept : add socket (%d)", fd);
#ifdef USE_GETNAMEINFO
			char    addrstr[INET6_ADDRSTRLEN], portstr[8];
			rc_gai = getnameinfo((struct sockaddr *)&saddr_c, len_saddr,
					addrstr, sizeof(addrstr), portstr, sizeof(portstr),
					NI_NUMERICHOST | NI_NUMERICSERV);
			if (rc_gai) {
				pr_err("Fail: getnameinfo():%s", gai_strerror(rc_gai));
				exit(EXIT_FAILURE);
			}
			if (saddr_c.ss_family == AF_INET) {
				pr_out("accept IPv4 (ip:port) (%s:%s)", addrstr, portstr);
			} else if (saddr_c.ss_family == AF_INET6) {
				pr_out("accept IPv6 (ip:port,scope) (%s:%s,%d)",
						addrstr, portstr,
						((struct sockaddr_in6 *)&saddr_c)->sin6_scope_id );
			}
#endif
		}

		for (i=1; i<cnt_fd_socket; i++) {
			if (FD_ISSET(fd_socket[i], &fds_read)) {
				pr_out("FD_ISSET: normal-inband");
				if ((ret_recv = recv(fd_socket[i], buf, sizeof(buf), 0)) == -1) {
					pr_err("fd(%d) recv() error (%s)", fd_socket[i], strerror(errno));
				} else {
					if (ret_recv == 0 || !strcmp("exit",buf)) {
						/* closed */
						pr_out("recv(fd=%d,n=%d) = %.*s", 
								fd_socket[i], ret_recv, ret_recv, buf);
						pr_out("fd(%d) : Session closed", fd_socket[i]);
						del_socket(fd_socket[i]);
					} else {
						/* normal */
						pr_out("recv(fd=%d,n=%d) = %.*s", 
								fd_socket[i], ret_recv, ret_recv, buf);
						if (-1 == (send_len =  send(fd_socket[i], 
								send_msg, strlen(send_msg), NULL)))
						{	
							pr_err("send() error");
							exit(EXIT_FAILURE);
						}
						pr_out("send(fd=%d,n=%d) = %.*s",
								fd_socket[i],send_len, send_len, send_msg);
					}
				}
			}
		} /* loop: for */
	}

	return 0;
}

/* Name : add_socket
 * Desc : add socket descriptor to internal fd array
 * Argv : int fd
 * Ret  : positive(if success, means array index), negative(if fail)
 */
int add_socket(int fd)
{
	if (cnt_fd_socket < MAX_FD_SOCKET) {
		fd_socket[cnt_fd_socket] = fd;
		return ++cnt_fd_socket;
	} else {
		return -1;
	}
}

/* Name : del_socket
 * Desc : substract socket descriptor from internal fd array
 * Argv : int fd
 * Ret  : positive(if success, means array index), negative(if fail)
 */
int del_socket(int fd)
{
	int	 i, flag;
	flag = 0; /* 1:found, 0:not found */

	close(fd);
	for (i=0; i<cnt_fd_socket; i++) {
		if (fd_socket[i] == fd) {
			if (i != (cnt_fd_socket-1)) fd_socket[i] = fd_socket[cnt_fd_socket-1];
			fd_socket[cnt_fd_socket-1] = -1;
			flag = 1;
            break;
		}
	}

	if (flag == 0) {
		return -1;
	}
	--cnt_fd_socket;

	return i;
}

/* Name : mk_fds
 * Desc : make fd set
 * Argv : fd_set *fds, int *a_fd_socket
 * Ret  : positive value (means biggest fd number)
 */
int mk_fds(fd_set *fds, int *a_fd_socket)
{
	int	i, fd_max;
	FD_ZERO(fds);
	for (i=0, fd_max = -1; i<cnt_fd_socket; i++) {
		fd_max = MAX(fd_max, a_fd_socket[i]);
		FD_SET(a_fd_socket[i], fds);
	}
	return fd_max;
}
