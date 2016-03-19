#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<stdint.h>

int g = 0;

void *myThreadFun(void *vargp)
{
	int myid = (int)(intptr_t)vargp;
	static int s = 0;

	++s;
	++g;
	printf("Thread ID: %d, Static: %d, Global: %d\n", myid, s, g);
}

int main()
{
	int i;
	pthread_t tid;
	
	for(i = 0; i < 3; i++) 
		pthread_create(&tid, NULL, myThreadFun, (void *)(intptr_t)i);
	pthread_exit(NULL);

	return 0;
}
