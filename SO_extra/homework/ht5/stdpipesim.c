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
#include <errno.h>

#define LEN(arr) ((int) (sizeof (arr) / sizeof (arr)[0]))
// Napisac program, ktory wykorzystuje lacza nienaane (pipe) 
// do polaczenia kilku polecen w potok - analogicznie do np. l s    | t r    a- z  AZ  | cu  t -f 1 -d' '
// Kolejne polecenia powinny byc zdefiniowane jako elementy tablicy w progra (z mozliwoscia ich latwej zmany) 
// i uruchamiane przez funkcje z rodziny exec, kazde polecenie w innym procesie.

int openPipe(int* fd) {
	if (pipe(fd) == -1) {
		printf("%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}
int makeFork() {
	int pid = fork();
	if (pid == -1) {
		exit(EXIT_FAILURE);
	}
	return pid;
}


int main(int argc, char* argv[]) {
	// pthread_mutex_t zamek = PTHREAD_MUTEX_INITIALIZER;
	char* args[][128] = {
	{ "ls", "-l", NULL, },
	{ "tr","a-z" ,"A-Z", NULL },
	{ "xargs", "echo", ">", "lol", NULL},
	// { "cut" , "-f1", "-d", " ", NULL},
	// 	{ "ls", "-l", ".", NULL } ,
	// 	{ "tail", "-n", "-2", NULL},
	// { "cat", "scores", NULL},

	};


	int progCount = LEN(args);
	switch (progCount) {
		case 0: {
			exit(EXIT_SUCCESS);
		}
		case 1: {
			execvp(**(args + 0), *(args + 0));
			exit(EXIT_SUCCESS);
		}
		case 2: {
			int fd[2];
			openPipe(fd);
			int pid = fork();
			if (pid == 0) {
				dup2(fd[1], STDOUT_FILENO);
				close(fd[0]);
				close(fd[1]);
				execvp(**(args + 0), *(args + 0));
				exit(EXIT_SUCCESS);
			}
			int pid2 = fork();
			if (pid2 == 0) {
				dup2(fd[0], STDIN_FILENO);
				close(fd[0]);
				close(fd[1]);
				execvp(**(args + 1), *(args + 1));
				// execlp("tail", "tail", "-n", "-2", NULL);
				exit(EXIT_SUCCESS);
			}
			close(fd[0]);
			close(fd[1]);
			waitpid(pid, NULL, 0);
			waitpid(pid2, NULL, 0);
		}
		default: {
			int fd[(progCount - 1) * 2];		// Need 2 descriptors for each | occurence 
			openPipe(fd);
			int pid[progCount];
			pid[0] = makeFork();
			if (pid[0] == 0) {
				dup2(fd[1], STDOUT_FILENO);		// STDOUT fd now points to pipe's write descriptor
				close(fd[1]);	// STDOUT already points at pipe's write fd
				close(fd[0]);	// Never reading input via this pipe
				execvp(**(args + 0), *(args + 0));
				exit(EXIT_SUCCESS);
			}
			for (int i = 1; i < progCount - 1; i++) {
				// waitpid(pid[i - 1], NULL, 0);
				pid[i] = makeFork();
				if (pid[i] == 0) {
					dup2(fd[1], STDOUT_FILENO);
					dup2(fd[0], STDIN_FILENO);
					close(fd[1]);		// STDOUT already points at pipe's write fd
					close(fd[0]);		// STDIN already points at pipe's read fd
					execvp(**(args + i), *(args + i));
					// execlp("tail", "tail", "-n", "-2", NULL);
					exit(EXIT_SUCCESS);
				}
			}
			pid[progCount - 1] = makeFork();
			if (pid[progCount - 1] == 0) {
				dup2(fd[0], STDIN_FILENO);
				close(fd[0]);
				close(fd[1]);
				execvp(**(args + progCount - 1), *(args + progCount - 1));
				// execlp("tail", "tail", "-n", "-2", NULL);
				exit(EXIT_SUCCESS);
			}
			close(fd[0]);
			close(fd[1]);
			for (int i = 0; i < progCount; i++) {
				waitpid(pid[i], NULL, 0);
			}

		}
	}


	const char* fileName = **(args + 2);
	char* const* fileArgs = *(args + 2);
	// execvp(**(args + 2), *(args + 2));



	exit(EXIT_SUCCESS);
}