#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

void *myThreadFun(void *vargp)
{
	sleep(1);
	printf("printing geeksquiz from thread\n");
	return NULL;
}

int main()
{
	pthread_t tid;
	
	printf("before thread\n");
	
	pthread_create(&tid, NULL, myThreadFun, NULL);
	pthread_join(tid, NULL);
	
	printf("after thread\n");
	
	return 0;
}
