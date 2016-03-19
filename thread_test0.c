#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

void *printThreadId(void *arg)
{
	printf("current thread id is %u\n", (unsigned)pthread_self());
}

int main(int argc, char *argv[])
{
	pthread_t pthread;
	
	pthread_create(&pthread, NULL, printThreadId, NULL);
	sleep(10);
	printf("main thread id is %u\n",(unsigned)pthread_self());

	return 0;
}
