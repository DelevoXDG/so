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
	if (access(pubFifoName, F_OK) == 0) {
		unlink(pubFifoName);
	}
	keep_running = 0;
	exit(EXIT_SUCCESS);
}



int main(int argc, char const* argv[]) {
	signal(SIGINT, sig_handler);
	// int fd[2];

	fd_set readCheck;
	fd_set errCheck;
	struct timeval timeout;
	mkfifo(pubFifoName, 0744);
	FD_ZERO(&readCheck);
	FD_ZERO(&errCheck);

	printf("Server online.\n");
	while (keep_running) {
		int pubFifo = open(pubFifoName, O_RDONLY);

		// char buf;
		// while (read(pubFifo, &buf, 1) > 0) {

		int x = 0;
		char pfName[PATH_MAX + 1];
		x = read(pubFifo, &pfName, PATH_MAX);
		if (x == 0) {
			break;
		}
		else {
			pfName[x] = '\0';
		}
		close(pubFifo);

		int privFifo = open(pfName, O_RDONLY);
		if (privFifo == -1) {
			printf("Lost connection to client\n");
			continue;
		}
		char receivedDir[PATH_MAX + 1];
		// printf("%i", x);
		printf("request received: %s\n", pfName);
		x = read(privFifo, &receivedDir, PATH_MAX);
		if (x == 0) {
			printf("Client disconnected\n");
			continue;
		}
		else {
			receivedDir[x] = '\0';
		}
		// printf("%i", x);
		close(privFifo);
		privFifo = open(pfName, O_WRONLY | O_RSYNC);
		FD_SET(privFifo, &readCheck);
		FD_SET(privFifo, &errCheck);

		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		int rv = select(privFifo + 1, &readCheck, NULL, &errCheck, &timeout);
		if (rv < 0) {
			// printf("Select failed\n");
			printf("Connection lost: select failed.\n");
			continue;
		}

		if (FD_ISSET(privFifo, &errCheck)) {
			printf("Connection lost: FD error\n");
			continue;
		}


		if (privFifo == -1) {
			printf("Connection lost.\n");
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
