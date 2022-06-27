#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define null NULL

int main(int argc, char const* argv[])
{
	const char queue[] = "./ala.txt";
	int targetFile = open(queue, O_WRONLY);
	int counter = 0;
	while (counter < 3) {
		char buffer[1600];
		scanf("%s", &buffer);
		write(targetFile, buffer, strlen(buffer) + 1);
		counter++;
	}
	close(targetFile);
	return 0;
}
