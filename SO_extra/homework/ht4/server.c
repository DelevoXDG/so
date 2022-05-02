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

#define null NULL
#define nullptr NULL

static volatile sig_atomic_t keep_running = 1;

static void sig_handler(int _) {
	printf("Server terminated.\n");
	(void)_;
	keep_running = 0;
	if (access(pubFifoName, F_OK) == 0) {
		unlink(pubFifoName);
	}
	exit(EXIT_SUCCESS);
}


int main(int argc, char const* argv[]) {
	signal(SIGINT, sig_handler);
	int fd[2];
	mkfifo(pubFifoName, 0744);

	while (1) {
		int pubFifo = open(pubFifoName, O_RDONLY);
		// char buf;
		// while (read(pubFifo, &buf, 1) > 0) {

		int x = 0;
		char pfName[PATH_MAX];
		x = read(pubFifo, &pfName, PATH_MAX);
		if (x == 0) {
			break;
		}
		close(pubFifo);

		pfName[x] = '\0';
		int privFifo = open(pfName, O_RDONLY);
		if (privFifo == -1) {
			printf("Lost connection to client\n");
			continue;
		}
		char receivedDir[PATH_MAX];
		// printf("%i", x);
		printf("request received: %s\n", pfName);
		x = read(privFifo, &receivedDir, 300);
		if (x == 0) {
			printf("Client disconnected\n");
			break;
		}
		receivedDir[x] = '\0';
		// printf("%i", x);
		close(privFifo);
		privFifo = open(pfName, O_WRONLY);
		if (privFifo == -1) {
			printf("Lost connection to client\n");
			continue;
		}
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
		printf("request handled:  %s\n", pfName);

	}

	unlink(pubFifoName);
	exit(EXIT_SUCCESS);
}
