#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stddef.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<sys/stat.h>
#include<errno.h>
#include<sys/types.h>

#define QLEN 10

int main(void) {
	int fd, size;
	struct sockaddr_un un;
	
	memset(&un, 0, sizeof(un));
	un.sun_family = AF_UNIX;
	strcpy(un.sun_path, "foo.socket");

	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		perror("socket error");
		exit(1);
	} 

	size = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);
	if ((bind(fd, (struct sockaddr *)&un, size)) < 0) {
		perror("bind error");
		exit(1);
	}

	printf("UNIX domain socket bound\n");
	exit(0);
}

/*
 * Create a server endpoint of a connection. Returns fd if all OK, <0 on error.
 */
int serv_listen(const char *name) 
{
	int fd, len, err, rval;
	struct sockaddr_un un;
	
	/* create a UNIX domain stream socket */
	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) 
		return(-1);
	unlink(name); // in case it already exists
	
	/* fill in socket address structure */
	memset(&un, 0, sizeof(un));
	un.sun_family = AF_UNIX;
	strcpy(un.sun_path, name);
	len = offsetof(struct sockaddr_un, sun_path) + strlen(name);

 	/* bind the name to the descriptor */
	if (bind(fd, (struct sockaddr *)&un, len) < 0) {
		rval = -2;
		goto errout;
	}		

	/* tell kernal we're a server.*/
	if (listen(fd, QLEN) < 0) {
		rval = -3;
		goto errout;
	}

	return(fd);

errout:
	err = errno;
	close(fd);
	errno = err;
	return(rval);

}

int serv_accept(int listenfd, uid_t *uidptr)
{
	int clifd, len, err, rval;
	time_t staletime;
	struct sockaddr_un un;
	struct stat statbuf;

	len = sizeof(un);
	if ((clifd = accept(listenfd, (struct sockaddr *)&un, &len)) < 0) 
		return(-1); // often errno = EINTR, if signal caught

	/* obtain the client's uid from its calling address */
	len -= offsetof(struct sockaddr_un, sun_path); //len of pathname
	un.sun_path[len] = 0; //null terminate	

	if (stat(un.sun_path, &statbuf) < 0) {
		rval = -2;
		goto errout;
	}		

	if (S_ISSOCK(statbuf.st_mode) == 0) {
		rval = -3;
		goto errout;
	}
	
  	if (uidptr != NULL) 
		*uidptr = statbuf.st_uid; //return uid of caller
	unlink(un.sun_path); //we're done with pathname now
	return(clifd);

errout:
	err = errno;
	close(clifd);
	errno = err;
	return(rval);
}
	
