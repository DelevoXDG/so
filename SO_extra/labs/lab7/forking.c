#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include	<stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#define null NULL

void orphansandzombie() {
	int x = 0;
	if (fork() == 0) {
		exit(0);	// Zobmie
	}
	else {
		if (fork() == 0) {
			for (int i = 0; i < 3; i++) {
				if (fork() == 0) {
					sleep(10);				// Sieroty
					exit(10);
				}
			}
			exit(0);
		}
		sleep(10);
		waitpid(-1, &x, 0);
		exit(0);
	}
	printf("%d\n", x);
}

void nums() {
	int pid_arr[10];
	int prev;
	for (int i = 1; i <= 10; i++) {
		int r = fork();
		pid_arr[i - 1] = r;
		if (r == 0) {
			if (i >= 2)
				waitpid(prev, null, 0);
			printf("%d ASDFASDFADF ASDFSADFA ASDFA 2380\n", i);
			// sleep(1);
			prev = r;
			exit(0);
		}
		// else {
		// 	waitpid(-1, null, 0);
		// 	// waitpid(r, null, 0);
		// }
		// for (int j = 1; j <= 10; j++) {
		// 	waitpid(pid_arr[j - 1], null, 0);
		// }
	}
}

int main(int argc, char* argv[]) {

	// orphansandzombie();
	nums();
	exit(0);
}