#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

#define MAXBUF 256
#define PORT 3389

int main(int argc, char **argv)
{
	int server_sockfd, client_sockfd;
	int client_len, n, err;
	char buf[MAXBUF];
	struct sockaddr_in cli_addr, serv_addr;
	
	client_len = sizeof(cli_addr);

	/* open socket */
	if ((server_sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror(" (server) socket() function error : ");
		exit(0);
	}
	else printf(" (server) socket open successfully, fd: %d \n", server_sockfd);

	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//serv_addr.sin_addr.s_addr = htonl("localhost");
	serv_addr.sin_port = htons(atoi(argv[1]));
	//serv_addr.sin_port = htons(PORT);
	
	if (bind (server_sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
	{
		printf(" (server) bind() failed \n");
		exit(0);
	} else printf(" (server) bind() ok, address: %s, port: %d \n", serv_addr.sin_addr.s_addr, serv_addr.sin_port);

	if (listen(server_sockfd, 2))
	{
		printf(" (server) listen() failed \n");
		exit(0);
	} else printf(" (server) listen \n");

	while(1)
	{
		memset(buf, 0x00, MAXBUF);
		client_sockfd = accept(server_sockfd, (struct sockaddr *)&cli_addr,
							&client_len);

		while(1)
		{
			if ((n = read(client_sockfd, buf, MAXBUF)) <= 0)
			{
				close(client_sockfd);
				break;
			}
			if (write(client_sockfd, buf, MAXBUF) <= 0)
			{
				perror("write error : ");
				close(client_sockfd);
				break;
			}
		}
		close(client_sockfd);
	}
}
