#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <limits.h>
#include "connection.h"
#include <sys/select.h>


#define null NULL
#define nullptr NULL

static volatile sig_atomic_t keep_running = 1;

static void sig_handler(int _) {
	printf("Server terminated.\n");
	(void)_;
	if (keep_running == 0) {
		exit(EXIT_FAILURE);
	}
	if (access(pubFifoName, F_OK) == 0) {
		unlink(pubFifoName);
	}
	keep_running = 0;
	exit(EXIT_SUCCESS);
}





int main(int argc, char const* argv[]) {
	signal(SIGINT, sig_handler);
	// int fd[2];


	FD_ZERO(&readCheck);
	FD_ZERO(&errCheck);
	FD_ZERO(&writeCheck);

	mkfifo(pubFifoName, 0744);
	printf("Server online.\n");
	while (keep_running) {
		printf("\tWaiting...\n");
		int pubFifo = open(pubFifoName, O_RDONLY);
		printf("123\n");
		if (pubFifo == -1) {
			printf("Client offline.\n");
		}
		char pfName[PATH_MAX + 1];

		int x = 0;
		x = read(pubFifo, &pfName, PATH_MAX);
		printf("%d\n", x);
		if (x == 0) {
			continue;
		}
		else {
			pfName[x] = '\0';
		}
		close(pubFifo);
		if (fork() == 0) {
			while (1) {
				pthread_mutex_lock(&RDL);
				int privFifo = open(pfName, O_RDONLY);
				if (privFifo == -1) {
					printf("Lost connection to client\n");
					break;
				}
				char receivedDir[PATH_MAX + 1];
				// printf("%i", x);
				printf("TODO: %s\n", pfName);
				x = read(privFifo, &receivedDir, PATH_MAX);
				if (x == 0) {
					close(privFifo);
					printf("Client disconnected by sending a empty string\n");
					break;
				}
				else {
					receivedDir[x] = '\0';
				}
				// printf("%i", x);
				close(privFifo);
				pthread_mutex_unlock(&RDL);
				pthread_mutex_lock(&WRL);
				privFifo = open(pfName, O_WRONLY);
				if (privFifo == -1) {
					break;
				}
				// privFifo = open(pfName, O_WRONLY | O_RSYNC);
				// FD_SET(privFifo, &readCheck);
				// FD_SET(privFifo, &errCheck);
				// timeout.tv_sec = 1;
				// timeout.tv_usec = 0;
				// rv = select(privFifo + 1, &readCheck, NULL, &errCheck, &timeout);


				// char fileNameCompact[x];
				// strcpy(fileNameCompact, receivedDir);
				// printf("%s\n", receivedDir);
				// printf("%s\n", fileNameCompact);
				if (fork() == 0) {
					dup2(privFifo, STDOUT_FILENO);
					close(privFifo);
					execlp("ls", "ls", receivedDir, null);
				}
				close(privFifo);
				pthread_mutex_unlock(&WRL);
				printf("DONE:  %s\n", pfName);
			}
			exit(EXIT_SUCCESS);
		}
		sleep(3);
	}

	unlink(pubFifoName);
	exit(EXIT_SUCCESS);
}
