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

// Napisac programy serwera oraz klienta (moze byc jednoczesnie uruchomionych kilku klientow), ktore zrealizuja nastepujaca komunikacje przy pomocy lacz nazwanych (kolejek FIFO):
// -serwer dziala w nieskonczonej petli odczytujac dane z 'publicznej' kolejki,
// -klient wysyla do serwera 'publiczna' kolejka nazwe swojej 'prywatnej' kolejki/kolejek do komunikacji,
// -klient wysyla 'prywatna' kolejka nazwe katalogu,
// -w odpowiedzi serwer odsyla do klienta jego 'prywatna' kolejka zawartosc tego katalogu (wynik polecenia ls),
// -klient wyswietla otrzymany wynik i dziala dalej w petli (moze wysylac kolejne nazwy katalogow), jesli chce zakonczyc polaczenie wysyla do serwera pusty ciag znakow i usuwa 'prywatna' kolejke/kolejki.
// Uwaga: w systemie wydzialowym utworzenie kolejki funkcja mkfifo nie jest mozliwe w katalogu domowym (brak uprawnien) ale dziala poprawnie w katalogu /tmp


// INFO
// SIGINT sends null string to force disconnection
// Sending empty dir path also forces disconnection
// Wrong path won't be returned


#include "connection.h"

#define null NULL
#define nullptr NULL
#define true 1
#define false 0

static volatile sig_atomic_t keep_running = 1;

typedef struct {
	char string[PATH_MAX + 1];
}String;

typedef struct {
	String* arr;
	size_t taken;
	size_t size;
} Array;

void initArray(Array* a, size_t initialSize) {
	a->arr = malloc(initialSize * sizeof(*(a->arr)));
	a->taken = 0;
	a->size = initialSize;
}
void insertArray(Array* a, String element) {
	if (a->taken >= a->size) {
		a->size *= 2;
		a->arr = realloc(a->arr, a->size * sizeof(*(a->arr)));
	}
	a->arr[a->taken++] = element;
}
void freeArray(Array* a) {
	if (a->arr == null) {
		return;
	}
	free(a->arr);
	a->arr = null;
	a->taken = 0;
	a->size = 0;
}

int getRand(const int lower, const int upper) {
	return (rand() % (upper - lower + 1)) + lower;
}
static Array queryArr;
static char pfName[PATH_MAX + 1];
static int privFifo;

void cleanAll() {
	int i = 0;
	for (i = 0; i < queryArr.taken; i++) {
		if (access(queryArr.arr[i].string, F_OK) == 0) {
			unlink(queryArr.arr[i].string);
		}
	}
	freeArray(&queryArr);
}
int is_valid_fd(int fd) {
	return fcntl(fd, F_GETFL) != -1 || errno != EBADF;
}
static void sig_handler(int _) {
	(void)_;
	if (keep_running == 0) {
		exit(EXIT_FAILURE);
	}
	keep_running = 0;
	// if (is_valid_fd(privFifo)) {
	// 	write(privFifo, "", 0);
	// 	close(privFifo);
	// }
	// else if (access(pfName, F_OK) == 0) {
	// 	privFifo = open(pfName, O_WRONLY);
	// 	// write(privFifo, "", 1);
	// 	write(privFifo, "", 0);

	// 	close(privFifo);
	// }
	privFifo = open(pfName, O_WRONLY);
	write(privFifo, "", 0);
	close(privFifo);

	printf("Client terminated.\n");

	cleanAll();
	exit(EXIT_SUCCESS);
}

int isValidName2(const char* str) {
	char bad_chars[] = "!@%^*~|\\/&?<>\"";
	int i = 0;
	if (strlen(str) == 0) {
		return 0;
	}
	for (i = 0; i < strlen(bad_chars); ++i) {
		if (strchr(str, bad_chars[i]) != NULL) {
			return 0;
		}
	}
	return 1;
}

int main(int argc, char const* argv[]) {
	signal(SIGINT, sig_handler);
	srand(time(NULL));
	pid_t thisPid = getpid();

	String* myStr;

	initArray(&queryArr, 4);
	int counter = 0;

	char privateId[PATH_MAX - 7 - 10 - strlen(appName) - strlen(privateExt)];
	// Section generare private fifo
	// Section make name
	// int privateId = getRand(100000, 999999);

	// do {
	// 	memset(&privateId[0], '\0', sizeof(privateId));
	// 	printf("Enter private fifo name: \n> ");

	// 	// scanf("%s", privateId);
	// 	fgets(privateId, PATH_MAX, stdin);
	// 	// privateId[strlen(privateId) - 1] = '\0';
	// 	sscanf(privateId, "%s", privateId);
	// 	// if ())
	// } while (isValidName2(privateId) == 0);


	// snprintf(pfName, sizeof(pfName), "%s%s_%d_%s_%d%s", privatePath, appName, thisPid, privateId, counter, privateExt);
	// snprintf(pfName, sizeof(pfName), "%s%s_%d_%s%s", privatePath, appName, thisPid, privateId, privateExt);
	// snprintf(pfName, sizeof(pfName), "%s%s_%d_%s%s", privatePath, appName, thisPid, privateId, privateExt);
	snprintf(pfName, sizeof(pfName), "%s%s_%d%s", privatePath, appName, thisPid, privateExt);
	// Section !make name
	String toDel;
	strncpy(toDel.string, pfName, strlen(pfName) + 1);
	insertArray(&queryArr, toDel);
	// Section! generate private fifo 
	mkfifo(pfName, 0777);
	// Section sending private queue 
	int pubFifo = open(pubFifoName, O_WRONLY);
	if (pubFifo == -1) {
		printf("Server offline.\n");
		cleanAll();
		// printf("%s\n", strerror(errno));
		exit(EXIT_SUCCESS);
	}

	// int privateId = getRand(100000, 999999);
	// char pfName[PATH_MAX + 1];
	// snprintf(pfName, sizeof(pfName), "%s%d-%dMFA%s", privatePath, thisPid, privateId, ".priv");
	// String toDel;
	// strncpy(toDel.string, pfName, strlen(toDel.string) + 1);
	// insertArray(&queryArr, toDel);
	// mkfifo(pfName, 0777);
	// printf("%s\n", pfName);

	write(pubFifo, pfName, strlen(pfName));
	close(pubFifo);
	// Section ! sending private queue 

	while (keep_running) {

		// Section sending directory
		char buf2[PATH_MAX + 1];
		char dirName[PATH_MAX + 1];
		memset(&buf2[0], '\0', sizeof(buf2));
		memset(&dirName[0], '\0', sizeof(buf2));
		// sleep(1);
		// scanf(&dirName, strlen(dirName));
		printf("Enter dir path\n> ");
		fgets(buf2, PATH_MAX, stdin);
		if (buf2[0] == '\n') {
			buf2[0] = '\0';
		}
		if (dirName[strlen(dirName) - 1] == '\n') {
			dirName[strlen(dirName) - 1] = '0';
		}

		sscanf(buf2, "%s[^\n]", dirName);
		privFifo = open(pfName, O_WRONLY);
		if (privFifo == -1) {
			// printf("%s\n", strerror(errno));
			break;
		}
		write(privFifo, dirName, strlen(dirName));
		close(privFifo);
		if (strlen(dirName) == 0) {
			printf("Exit\n");
			break;
		}

		char buf[17];
		privFifo = open(pfName, O_RDONLY);

		while (1) {
			int x = read(privFifo, &buf, 16);
			if (x == 0) {
				break;
			}
			buf[x] = '\0';
			printf("%s", buf);
		}

		close(privFifo);

		// unlink(pfName);
		// sleep(1);
		counter++;
	}
	cleanAll();
	exit(EXIT_SUCCESS);
}