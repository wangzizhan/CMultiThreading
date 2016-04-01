/*
 * reengineering server.c
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>

#define MAXLINE 80
#define SERV_PORT 9000

int main(int argc, char *argv[])
{
	struct sockaddr_in servaddr, cliaddr;
	socklen_t cliaddr_len;
	int listenfd, connfd;
	char str[INET_ADDRSTRLEN];
	char buf[MAXLINE];
	int i, n;
	int opt = 1;
	
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);	

	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));	
	
	listen(listenfd, 20);
		
	while (1) {
		cliaddr_len = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
		n = fork();
		if (n == -1) {
			perror("call to fork");
			exit(1);
		} else if (n == 0) { 
			while (1) {
				//close(listenfd);
				n = read(connfd, buf, MAXLINE);
				if (n == 0) {
					printf("client side has been closed.\n");
					break;
				}	
				inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str));
				printf("received from %s at PORT %d\n", str, ntohs(cliaddr.sin_port));

				for (i = 0; i < n; i++) 
					buf[i] = toupper(buf[i]);
				write(connfd, buf, n);
			}
			close(connfd);
			exit(0);
		} else 
			close(connfd);
	}
}
