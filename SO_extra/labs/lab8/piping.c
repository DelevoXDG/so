#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define null NULL

int doPipe() {
	char buffer[100];
	int fd[2];
	pipe(fd);
	if (fork() == 0) {
		close(fd[1]);
		read(fd[0], buffer, 16);
		printf("%s", buffer);
	}
	else {
		close(fd[0]);
		char msg[10] = "Cokolwiek";
		write(fd[1], msg, 10);
	}
	return 0;
}

int lstail() {
	char buffer[100];
	int fd[2];
	pipe(fd);
	if (fork() == 0) {
		close(fd[1]);
		dup2(fd[0], 0);
		execlp("tail", "tail", "-n", "-2", null);
		printf("alive\n");
	}
	else {
		close(fd[0]);
		dup2(fd[1], 1);
		execlp("ls", "ls", "-l", null);
		printf("b\n");
	}
	return 0;
}

int fifoPipe() {
	const char queue[] = "./ala.txt";
	// mkfifo("ala.txt", 0600);
	int fifo = open(queue, O_RDONLY);
	char buffer[17];
	while (1) {
		int x = read(fifo, buffer, 16);
		if (x == 0) break;
		buffer[x] = '\0';
		int i = 0;
		for (i = 0; i < 17; i++) {
			if (buffer[i] == '\0')
				printf(" ");
			else
				printf("%c", buffer[i]);
		}

	}
	close(fifo);

	return 0;
}

int main(int argc, char const* argv[]) {

	// doPipe();
		// idkwhat();
	// fifoPipe();
	lstail();
	return 0;

}
