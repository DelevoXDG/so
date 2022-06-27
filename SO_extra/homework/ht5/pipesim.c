// simulates cmd | cmd | ... 

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define LEN(arr) ((int) (sizeof (arr) / sizeof (arr)[0]))
#define CMD_ARGC_MAX 128 		// CMD string limit, commands can't have array of more strings than this variable

int openPipe(int* fd) {
	if (pipe(fd) == -1) {
		printf("%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return 0;
}
int makeFork() {
	int pid = fork();
	if (pid == -1) {
		printf("%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return pid;
}
int closeAllPipes(int* fd, int len) {
	for (int i = 0; i < len; i++) {
		close(fd[i]);
	}
	return 0;
}


int main(int argc, char* argv[]) {
// Napisac program, ktory wykorzystuje lacza nienaane (pipe) 
// do polaczenia kilku polecen w potok - analogicznie do np. l s    | t r    a- z  AZ  | cu  t -f 1 -d' '
// Kolejne polecenia powinny byc zdefiniowane jako elementy tablicy w progra (z mozliwoscia ich latwej zmany) 
// i uruchamiane przez funkcje z rodziny exec, kazde polecenie w innym procesie.

	char* args[][CMD_ARGC_MAX] = {
	//1
	{ "ls", "-l", NULL },
	{ "tr","a-z" ,"A-Z", NULL },
	{ "cut" , "-f1", "-d", " ", NULL },
	{ "grep", "-n", "R", NULL },
	{ "tail", "-n", "-5", NULL},


	//2
	// { "ls", "-l", ".", NULL } ,
	// { "tail", "-n", "-2", NULL},

	//3
	// {"ps", "-le", NULL},
	// {"awk", "{if (NR !=1 && $7>70) {print $4}}", NULL},
	// {"wc", "-l", NULL}

	//4
	// {"cat", "hello world", NULL},
	// {"grep", "world", NULL},
	// {"wc", "-w", NULL},
	};


	int progCount = LEN(args);
	switch (progCount) {
		case 0: {
			break;
		}
		case 1: {
			pid_t pid = makeFork();
			if (pid == 0) {
				execvp(**(args + 0), *(args + 0));
			}
			waitpid(pid, NULL, 0);
			break;
		}
		default: {
			int fd[(progCount - 1) * 2];		// Need 2 descriptors for each | occurence 
			for (int i = 0; i < progCount - 1; i++) {
				openPipe(fd + 2 * i);
			}
			pid_t pid[progCount];
			int last = progCount - 1;

			pid[0] = makeFork();

			if (pid[0] == 0) {
				dup2(fd[1], STDOUT_FILENO);		// STDOUT fd now points to pipe's write descriptor
				closeAllPipes(fd, 2 * last);
				execvp(**(args + 0), *(args + 0));
			}
			for (int i = 1; i < last; i++) {
				pid[i] = makeFork();
				if (pid[i] == 0) {
					dup2(fd[2 * i - 2], STDIN_FILENO);
					dup2(fd[2 * i + 1], STDOUT_FILENO);
					closeAllPipes(fd, 2 * last);
					execvp(**(args + i), *(args + i));
				}
			}

			pid[last] = makeFork();
			if (pid[last] == 0) {
				dup2(fd[(last - 1) * 2], STDIN_FILENO);
				closeAllPipes(fd, 2 * last);
				execvp(**(args + last), *(args + last));\
			}
			closeAllPipes(fd, 2 * last);
			for (int i = 0; i < progCount; i++) {
				waitpid(pid[i], NULL, 0);
			}
			break;
		}
	}

	exit(EXIT_SUCCESS);
}