#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<stdio.h>
extern int errno;


int main(int argc, char* argv[]) {
	const int N = 100;
	char buffer[N];

	int source = open(argv[1], O_RDONLY);
	int dest = open(argv[2], O_WRONLY | O_CREAT | O_EXCL);

	if (source == -1) {
		printf("ERROR reading source file\n");
		// exit(1);
		return 2;
	}
	if (dest == -1) {
		printf("ERROR creating destination file\n");
		// exit(1);
		return 3;
	}

	int x = 0;
	while ((x = read(source, buffer, N)) != 0) {
		if (x == -1) {
			printf("ERROR reading\n");
		}
		else {
			int y = write(dest, buffer, x);
			if (y == -1) {
				printf("ERROR writing\n");
			}
		}
	}



	close(source);
	close(dest);
	return 0;
}