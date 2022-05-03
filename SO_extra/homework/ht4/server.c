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



static int rv;
static fd_set readCheck;
static fd_set errCheck;
static struct timeval timeout;

int prepareWrite(const char* name) {
	int fd = open(name, O_WRONLY | O_RSYNC);
	FD_SET(fd, &readCheck);
	FD_SET(fd, &errCheck);
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	rv = select(fd + 1, &readCheck, NULL, &errCheck, &timeout);
	if (rv < 0) {
			// printf("Select failed\n");
		printf("Connection lost: select failed.\n");
		return -1;
	}

	if (FD_ISSET(fd, &errCheck)) {
		printf("Connection lost: FD error\n");
		return -1;
	}

	if (fd == -1) {
		printf("Connection lost.\n");
		return -1;
	}
	return fd;
}

int readTo(const char* name, char* buffer) {
	int fd = open(name, O_RDONLY);
	FD_SET(fd, &readCheck);
	FD_SET(fd, &errCheck);
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	rv = select(fd + 1, &readCheck, NULL, &errCheck, &timeout);

	if (rv < 0) {
		printf("Cannot read: select failed\n");
		return -1;
	}

	if (FD_ISSET(fd, &errCheck)) {
		printf("Cannot read: FD error\n");
		return -1;
	}

	if (FD_ISSET(fd, &readCheck)) {
		memset(buffer, 0, sizeof(buffer));
		int x = read(fd, buffer, sizeof(buffer));
		if (x < 0) {
			printf("Read failed\r\n");
			return -1;
		}
		if (x == 0) {
			return -2;
		}
		buffer[x] = '\0';
		return 0;
	}
	close(fd);
	return -1;
}

int main(int argc, char const* argv[]) {
	signal(SIGINT, sig_handler);
	// int fd[2];


	FD_ZERO(&readCheck);
	FD_ZERO(&errCheck);

	mkfifo(pubFifoName, 0744);
	printf("Server online.\n");
	while (keep_running) {
		printf("Waiting...\n");

		char pfName[PATH_MAX + 1];
		int pubFifo = readTo(&pubFifoName[0], &pfName[0]);
		if (pubFifo == -1 || pubFifo == -2) {
			continue;
		}
		printf("%d\n", pubFifo);


		// int pubFifo = open(pubFifoName, O_RDONLY);

		// rv = read(pubFifo, &pfName, PATH_MAX);
		// if (rv == 0) {
		// 	break;
		// }
		// else {
		// 	pfName[rv] = '\0';
		// }
		// close(pubFifo);

		int privFifo = open(pfName, O_RDONLY);
		if (privFifo == -1) {
			printf("Lost connection to client\n");
			continue;
		}
		char receivedDir[PATH_MAX + 1];
		// printf("%i", x);
		printf("request received: %s\n", pfName);
		rv = read(privFifo, &receivedDir, PATH_MAX);
		if (rv == 0) {
			printf("Client disconnected\n");
			continue;
		}
		else {
			receivedDir[rv] = '\0';
		}
		// printf("%i", x);
		close(privFifo);

		privFifo = prepareWrite(&pfName[0]);
		if (privFifo == -1) {
			continue;
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
		printf("request handled:  %s\n", pfName);
	}

	unlink(pubFifoName);
	exit(EXIT_SUCCESS);
}
