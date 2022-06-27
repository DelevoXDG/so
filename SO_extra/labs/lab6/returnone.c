#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<stdio.h>
extern int errno;



int main(int argc, char* argv[]) {
	int desc = open("test.txt", O_RDWR | O_CREAT);
	int randompwd = open("/dev/random", O_RDONLY);
	printf("%d\n", desc);
	printf("%d\n", randompwd);
	char file_content[100];
	read(randompwd, file_content, 64);
	write(desc, file_content, 64);

	// Desktryptory
	// -1 - error
	// 0 - stdin
	// 1 - stdout
	// 2 - stderr
	close(desc);
	close(randompwd);



	return 0;
}