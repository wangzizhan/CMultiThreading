/*
 * Read characters from client, convert them to uppercase, then transfer them to client.
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

int main(void)
{
	struct sockaddr_in servaddr, cliaddr;
	socklen_t cliaddr_len;
	int listenfd, connfd;
	char buf[MAXLINE]; //store characters
	char str[INET_ADDRSTRLEN]; //store ip
	int i, n;

	//listenfd = socket(AF_INET, SOCK_STREAM, 0); //ipv4 tcp 
	listenfd = Socket(AF_INET, SOCK_STREAM, 0); //ipv4 tcp 

	int opt = 1;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	//bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	//listen(listenfd, 20);
	Listen(listenfd, 20);
	
	printf("Accepting connections...\n");			
	//while (1) {
	//	cliaddr_len = sizeof(cliaddr);
	//	//connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
	//	connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
	//	
	//	//n = read(connfd, buf, MAXLINE);
	//	n = Read(connfd, buf, MAXLINE);
	//	
	//	inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)); //gc 
	//	printf("received from %s at PORT %d\n", str, ntohs(cliaddr.sin_port));
	//	for (i = 0; i < n; i++) 
	//		buf[i] = toupper(buf[i]);
	//	//write(connfd, buf, n);
	//	Write(connfd, buf, n);
	//	//close(connfd);
	//	Close(connfd);
	//}

	while (1) {
		cliaddr_len = sizeof(cliaddr);
		connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
		n = fork();

		if (n == -1) {
			perror("call to fork");
			exit(1);
		} else if (n == 0) {
			Close(listenfd);
			while (1) {
				n = Read(connfd, buf, MAXLINE);
				if (n == 0) {
					printf("the other size has been close.\n");
					break;
				} 
				inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str));
				printf("received from %s at PORT %d\n", str, ntohs(cliaddr.sin_port));
				for (i = 0; i < n; i++) 
					buf[i] = toupper(buf[i]);
				Write(connfd, buf, n);
			}
			close(connfd);
			exit(0);
		} else 
			Close(connfd);

	}	
}

 
