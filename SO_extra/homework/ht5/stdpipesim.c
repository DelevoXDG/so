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

int main(int argc, char* argv[]) {
	// pthread_mutex_t zamek = PTHREAD_MUTEX_INITIALIZER;
	char* args[][10] = {
		{ "ls", "-l", ".", NULL } ,
		{ "tail", "-n", "-2", NULL},
		{ "cat", "scores", NULL},
	};
	// char* cat_args[] = { "cat", "scores", NULL };
	// char* grep_args[] = { "grep", "amogus", NULL };

	// for (int i = 0; i < LEN(args) - 1; i++) {
	// printf("%d\n", i);
	// int fd[2];
	// pipe(fd);
	// // pipe(fd + 2);
	// if (fork == 0) {

	// }
	// if (fork == 0) {
	// 	close(fd[1]);
	// 	dup2(fd[0], STDIN_FILENO);
	// 	// execvp(*(args[i + 1]), args[i + 1]);
	// 	execvp(*cat_args, cat_args);
	// 	printf("alive\n");

	// }
	// else {
	// 	close(fd[0]);
	// 	dup2(fd[1], STDOUT_FILENO);
	// 	// char** temp = &args[i][0];
	// 	// execvp(*(args[i]), args[i]);
	// 	execvp(*grep_args, grep_args);
	// 	printf("b\n");
	// }

// }


	const char* fileName = **(args + 2);
	char* const* fileArgs = *(args + 2);
	// execvp(**(args + 2), *(args + 2));
	int fd[2];
	if (pipe(fd) == -1) {
		printf("%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
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
		printf("Alive\n");
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


	exit(EXIT_SUCCESS);
}