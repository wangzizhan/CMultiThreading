/*
 * Read characters from console, transfer them to server, then reveice result from server.
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h> //internet protocol family
#include "wrap.h"

#define MAXLINE 80
#define SERV_PORT 8100

int main(int argc, char *argv[]) 
{
	struct sockaddr_in servaddr;
	char buf[MAXLINE]; //store characters
	int sockfd, n;
	char *str;

	if (argc != 2) {
		fputs("usage: ./client message\n", stderr);
		exit(-1);
	}
	str = argv[1];

	//sockfd = socket(AF_INET, SOCK_STREAM, 0);
	sockfd = Socket(AF_INET, SOCK_STREAM, 0);
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
	servaddr.sin_port = htons(SERV_PORT);
	
	//connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	Connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	
	//while(1);
	
	//write(sockfd, str, strlen(str));
	Write(sockfd, str, strlen(str));
	
	//n = read(sockfd, buf, MAXLINE);
	n = Read(sockfd, buf, MAXLINE);
	printf("Response from server:\n");
	//write(STDOUT_FILENO, buf, n);
	Write(STDOUT_FILENO, buf, n);
	
	printf("\n");	
	//close(sockfd);
	Close(sockfd);
	return 0;
} 
