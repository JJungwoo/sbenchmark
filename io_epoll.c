/* vim: set ts=4 sw=4: */
/* Filename    : io_epoll.c
 * Description : I/O multiplexing implementation with epoll
 * Author      : SunYoung Kim <sunyzero@gmail.com>
 * Notes       : when define ENABLE_MSG_OOB, it allow to receive OOB data
 *
 *               Run the following command after building this example.
 *                  $ ./io_epoll 5000
 *
 *               You can connect to the example server with nc or telnet.
 *               if nc : 
 *                  $ nc 127.0.0.1 5000
 *
 *               if telnet :
 *                  $ telnet 127.0.0.1 5000
 */
#define _XOPEN_SOURCE   700
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
//#include "stdalsp.h"
#include "jlib.h"

#define LISTEN_BACKLOG	256

#define ADD_EV(a, b)	if (add_ev(a, b) == -1) { pr_err("Fail: add_ev"); exit(EXIT_FAILURE); }
#define DEL_EV(a, b)	if (del_ev(a, b) == -1) { pr_err("Fail: del_ev"); exit(EXIT_FAILURE); }

#define BUF_SIZE 4096
#define SMESSAGE "success"

const int	max_ep_events	= 256;

int	epollfd;		/* epoll fd */

int fcntl_setnb(int fd);		/* set non-blocking mode */
int add_ev(int efd, int fd);	/* Add epoll variable to epollfd */
int del_ev(int efd, int fd); /* delete epoll variabe from epollfd */

int main(int argc, char *argv[])
{
    /* network var			*/
    socklen_t	len_saddr;
    int		fd, fd_listener;
    int		ret_recv, send_len;
    char	*port, buf[1024], send_msg[BUF_SIZE];
    /* I/O multiplexing var */
    struct epoll_event	*ep_events;
    int		ret_poll;
    /* logical var 			*/
    int		i;

    if (argc > 2) {
        printf("%s [port number]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if (argc == 2) {
        port = strdup(argv[1]);
    } else {
        port = strdup("0");	/* random port */
    }
	// JJ
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

    fcntl_setnb(fd_listener); /* set nonblock flag */

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

    if ((epollfd = epoll_create(1)) == -1) {
        /* error */
        exit(EXIT_FAILURE);
    }
    if ((ep_events = calloc(max_ep_events, sizeof(struct epoll_event))) == NULL) {
        /* error */
        exit(EXIT_FAILURE);
    }

    ADD_EV(epollfd, fd_listener);
    while (1) {
        /* waiting for data (no timeout) */
        pr_out("Epoll waiting ...");
        if ((ret_poll = epoll_wait(epollfd, ep_events, max_ep_events, -1)) == -1) {
            /* error */
        }
        pr_out("Epoll return (%d)", ret_poll);

        /* Are there data to read? */
        for (i=0; i<ret_poll; i++) {
            /* event : EPOLLIN */
            if (ep_events[i].events & EPOLLIN) {
                /* Is New connection ? */
                if (ep_events[i].data.fd == fd_listener) {
                    struct sockaddr_storage saddr_c;
                    while(1) {
                        len_saddr = sizeof(saddr_c);
                        fd = accept(fd_listener, (struct sockaddr *)&saddr_c, &len_saddr);
                        if (fd == -1) {
                            if (errno == EAGAIN) { /* no more new connection */
                                break;
                            }
                            pr_err("Error get connection from listen socket");
                            /* need error processing */
                            break;
                        }
                        fcntl_setnb(fd);
                        ADD_EV(epollfd, fd);
                        pr_out("accept : add socket (%d)", fd);
                    }
                } 
                /* normal band */
                else {
                    if ((ret_recv = recv(ep_events[i].data.fd, buf, sizeof(buf), 0)) == -1) {
                        /* error */
                    } else {
                        if (ret_recv == 0 || !strcmp("exit", buf)) {
                            /* closed */
							pr_out("recv(fd=%d,n=%d) = %.*s",
									ep_events[i].data.fd, ret_recv, ret_recv, buf);
                            pr_out("fd(%d) : Session closed", ep_events[i].data.fd);
                            DEL_EV(epollfd, ep_events[i].data.fd);
                        } else {

                            /* normal */
                            pr_out("recv(fd=%d,n=%d) = %.*s", 
                                    ep_events[i].data.fd, ret_recv, ret_recv, buf);
							if (-1 == (send_len = send(ep_events[i].data.fd, 
								send_msg, strlen(send_msg), NULL)))
							{
								perror("send() error");
								//pr_err("send() error");
								exit(EXIT_FAILURE);
							}	
							pr_out("send(fd=%d,n=%d) = %.*s",
									ep_events[i].data.fd, send_len, send_len, send_msg);
                        }
                    }
                } /* end : else */
#ifdef ENABLE_MSG_OOB
            } else if (ep_events[i].events & EPOLLPRI) {
                pr_out("EPOLLPRI : Urgent data detected");

                if ((ret_recv = recv(ep_events[i].data.fd, buf, 1, MSG_OOB)) == -1) {
                    /* error */
                }
                pr_out("recv(fd=%d,n=1) = %.*s (OOB)", ep_events[i].data.fd, 1, buf);
#endif
            } else if (ep_events[i].events & EPOLLERR) {
                /* error */
            } else {
                pr_out("fd(%d) epoll event(%d) err(%s)", 
                        ep_events[i].data.fd, ep_events[i].events, strerror(errno));
            }
        } /* loop : for */
    } /* loop : while */

    return 0;
}

/* Name : add_ev
 * Desc : add socket descriptor to epoll
 * Argv : int efd, int fd
 * Ret  : zero(if success), negative(if fail)
 */
int add_ev(int efd, int fd)
{
    struct epoll_event ev;

    /* if want to use edge trigger, add EPOLLET to events */
#ifdef ENABLE_MSG_OOB	
    ev.events = EPOLLIN | EPOLLPRI;
#else
    ev.events = EPOLLIN ;
#endif
    ev.data.fd = fd;
    if (epoll_ctl(efd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        pr_out("fd(%d) EPOLL_CTL_ADD  Error(%d:%s)", fd, errno, strerror(errno));
        return -1;
    }

    return 0;
}

/* Name : del_ev
 * Desc : delete socket descriptor from epoll
 * Argv : int efd, int fd
 * Ret  : zero(if success), negative(if fail)
 */
int del_ev(int efd, int fd)
{
    if (epoll_ctl(efd, EPOLL_CTL_DEL, fd, NULL) == -1) {
        pr_out("fd(%d) EPOLL_CTL_DEL Error(%d:%s)", fd, errno, strerror(errno));
        return -1;
    }
    close(fd);
    return 0;
}

/* Name : fcntl_setnb
 * Desc : fcntl(O_NONBLOCK)
 * Argv : int fd
 * Ret  : zero(if success), negative(if fail)
 */
int fcntl_setnb(int fd)
{
    /* only influence about O_ASYNC, O_APPEND, O_NONBLOCK on Linux-specific */
    if (fcntl(fd, F_SETFL, O_NONBLOCK | fcntl(fd, F_GETFL)) == -1) {	
        return errno;
    }
    return 0;
}
