#include <sys/types.h>  
#include <sys/stat.h>  
#include <sys/socket.h>  
#include <signal.h>  
#include <unistd.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  

#define MAXLINE 1024 
int main(int argc, char **argv) 
{ 
    int server_sockfd, client_sockfd; 
    int state, client_len; 
    int pid; 
 
    struct sockaddr_in clientaddr, serveraddr; 
 
    char buf[MAXLINE]; 
 
    if (argc != 2) 
    { 
        printf("Usage : %s [port]\n", argv[0]); 
	return 1;
    }  
 
    // internet 스트림 소켓을 만들도록 한다.  
    if ((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        perror("socket error : "); 
        exit(0); 
    }  
    bzero(&serveraddr, sizeof(serveraddr)); 
    serveraddr.sin_family = AF_INET; 
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    serveraddr.sin_port = htons(atoi(argv[1])); 
 
    state = bind(server_sockfd , (struct sockaddr *)&serveraddr, sizeof(serveraddr)); 
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
     
    signal(SIGCHLD, SIG_IGN); 
    while(1) 
    { 
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&clientaddr, 
                               &client_len); 
        if (client_sockfd == -1) 
        { 
            perror("Accept error : "); 
            exit(0); 
        } 
        printf("Accept Success!!\n");
        pid = fork(); 
		printf("%d count client csockfd: %d \n",pid, client_sockfd);
        if (pid == 0)  
        { 
            while(1) 
            { 
                memset(buf, 0x00, MAXLINE); 
                if (read(client_sockfd, buf, MAXLINE-1) <= 0) 
                { 
                    close(client_sockfd); 
                    exit(0); 
                } 
                printf("(%d) > %s", client_sockfd, buf);
                write(client_sockfd, buf, strlen(buf)); 
            } 
        } 
        if (pid == -1) 
        { 
            perror("fork error : "); 
	    return 1;
        } 
    } 
    close(client_sockfd); 
} 
