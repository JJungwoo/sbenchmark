#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define FD_MAX_SIZE 1024
int main(int argc, char **argv)
{
    int server_sockfd, client_sockfd, sockfd;
    int state, client_len;
    int pid;
    int i, maxi, maxfd;

    int client[FD_MAX_SIZE];

    FILE *fp;
    struct sockaddr_in clientaddr, serveraddr;

    struct timeval tv;
    fd_set readfds, otherfds, allfds;

    int current_cli_num;

    char buf[255];
    char line[255];

    if (argc != 2)
    {
        printf("Usage : ./zipcode [port]\n");
        printf("예    : ./zipcode 4444\n");
        exit(0);
    }

    memset(line, 0x00, 255);
    state = 0;

    current_cli_num = 3;

    // 주소 파일을 읽어들인다. 
    /*
	if ((fp = fopen("zipcode.txt", "r")) == NULL)
    {
        perror("file open error : ");
        exit(0);
    }
	*/

    if ((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket error : ");
        exit(0);
    }
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(atoi(argv[1]));

    state = bind (server_sockfd, (struct sockaddr *)&serveraddr,
                  sizeof(serveraddr));
    if (state == -1)
    {
        perror("bind error : ");
        exit(0);
    }

    state = listen(server_sockfd, 5);
    if (state == -1)
    {
        perror("listen error : ");
        exit(0);
    }


    //client_sockfd = server_sockfd;

    maxi = -1;
    maxfd = server_sockfd;
    for (i = 0; i < FD_MAX_SIZE; i++)
    {
        client[i] = -1;
    }

    FD_ZERO(&readfds);
    FD_SET(server_sockfd, &readfds);

    while(1)
    {

        allfds = readfds;

        state = select(maxfd + 1 , &allfds, NULL, NULL, NULL);

        if (FD_ISSET(server_sockfd, &allfds))
        {
            client_len = sizeof(clientaddr);
            client_sockfd = accept(server_sockfd,
                    (struct sockaddr *)&clientaddr, &client_len);
            for (i = 0; i < FD_MAX_SIZE; i++)
            {
                if (client[i] < 0)
                {
                    client[i] = client_sockfd;
                    printf("%d : %d\n", i, client_sockfd);
                    break;
                }
            }

            printf("accept [%d]\n", client_sockfd);
            if (i == FD_MAX_SIZE)
            {
                perror("too many clients\n");
            }

            FD_SET(client_sockfd,&readfds);
    
            if (client_sockfd > maxfd)
                maxfd = client_sockfd;

            if (i > maxi)
                maxi = i;

            if (--state <= 0)
                continue;
        }
    
        printf("maxi %d\n", maxi);
        for (i = 0; i <= maxi; i++)
        {
            if ((sockfd = client[i]) < 0)
            {
                continue;
            }

            printf("maxi %d\n", maxi);
            if (FD_ISSET(sockfd, &allfds))
            {
                printf("ok read\n");
                memset(buf, 0x00, 255);
                if (read(sockfd, buf, 255) <= 0)
                {
                    close(sockfd);
                    perror("Close sockfd : ");
                    FD_CLR(sockfd, &readfds);
                    client[i] = -1;
                }
                else
                {
                    printf("[%d] %s\n", sockfd, buf);
                }

                if (--state <= 0)
                    break;
            }
        }
    }
	return 0;
}
