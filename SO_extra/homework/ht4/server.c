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
#include <sys/select.h>

#include "connection.h"

#define null NULL
#define nullptr NULL

static volatile sig_atomic_t keep_running = 1;

static void sig_handler(int _) {
	(void)_;
	printf("[!] Server terminated.\n");
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

	mkfifo(pubFifoName, 0744);
	printf("[!] Server online.\n");
	while (keep_running) {
		// printf("Waiting...\n");
		int pubFifo = open(pubFifoName, O_RDONLY);
		if (fork() == 0) {
			if (pubFifo == -1) {
				printf("[!] Client offline.\n");
			}
			char pfName[PATH_MAX + 1];

			int x = 0;
			x = read(pubFifo, &pfName, PATH_MAX);
			// printf("%d\n", x);
			if (x == 0) {
				exit(EXIT_SUCCESS);
				break;
			}
			else {
				pfName[x] = '\0';
			}
			close(pubFifo);
			printf("[+] Client\t%s connected\n", pfName);
			int counter = 0;
			while (1) {
				int privFifo = open(pfName, O_RDONLY);
				if (privFifo == -1) {
					printf("[-] Lost connection to client\n");

					break;
				}
				char receivedDir[PATH_MAX + 1];
				// printf("%i", x);
				x = read(privFifo, &receivedDir, PATH_MAX);
				counter++;
				if (x == 0) {
					close(privFifo);
					printf("[!] Client\t%s\t<%d> sent an empty string\n", pfName, counter);
					printf("[-] Client\t%s\t<%d> disconnected\n", pfName, counter);
					break;
				}
				else {
					receivedDir[x] = '\0';
				}
				printf("[!] TODO:\t%s\t<%d>\n", pfName, counter);
				// printf("%i", x);
				close(privFifo);

				privFifo = open(pfName, O_WRONLY);
				if (privFifo == -1) {
					printf("[-] Lost connection to client\n");
					break;

				}
				pid_t pid = fork();
				if (pid == 0) {
					dup2(privFifo, STDOUT_FILENO);
					close(privFifo);
					execlp("ls", "ls", receivedDir, null);
				}
				waitpid(pid, NULL, 0);			// Note very important to wait for client to wait till process ends to avoid RD/WR collision 
				close(privFifo);
				printf("[@] DONE:\t%s\t<%d>\n", pfName, counter);
			}
			exit(EXIT_SUCCESS);
		}
		close(pubFifo);
	}

	unlink(pubFifoName);
	exit(EXIT_SUCCESS);
}