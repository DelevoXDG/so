#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <pthread.h>

static const int M = 5;
static int  x = 0;
static pthread_mutex_t zamek = PTHREAD_MUTEX_INITIALIZER;

void* fun(void* arg) {
	for (int i = 0; i < M; i++) {
		pthread_mutex_lock(&zamek);
		x++;
		pthread_mutex_unlock(&zamek);
		printf("%d\n", x);
	}
	// pthread_mutex_unlock(&zamek);
	pthread_exit(NULL);
}




int main(int argc, char* argv[]) {

	// pthread_create(pthread *w, )
	const int TREAD_NUM = 4;
	pthread_t thread_id[TREAD_NUM];
	printf("Before Threads\n");
	for (int i = 0; i < TREAD_NUM; i++) {
		pthread_create(thread_id + i, NULL, fun, NULL);
	}
	for (int i = 0; i < TREAD_NUM; i++) {
		pthread_join(*(thread_id + i), NULL);
	}


	printf("After Threads\n");
	exit(EXIT_SUCCESS);
}