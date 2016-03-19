#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<string.h>

pthread_t t_id;

void printf_tid_pid(const char *s)
{
	pid_t pid;
	pthread_t tid;
	
	pid = getpid();
	tid = pthread_self();
	printf("%s pid %u tid %u (0x%x)\n", s, (unsigned)pid, (unsigned)tid, (unsigned)tid);
}

void *thread_function(void *arg)
{
	printf_tid_pid("new thread: ");
	return ((void*)0);
}

int main(int argc, char *argv[]) 
{
	int err;
	err = pthread_create(&t_id, NULL, thread_function, NULL);
	if(err != 0) {
		printf("create thread fail: %s\n", strerror(err));
	}
	printf_tid_pid("main thread");
	sleep(1);
	return 0;
} 
