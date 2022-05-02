#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <errno.h>

// Napisać programy serwera oraz klienta (może być jednocześnie uruchomionych kilku klientów), które zrealizują następującą komunikację przy pomocy łącz nazwanych (kolejek FIFO):
// -serwer działa w nieskończonej pętli odczytując dane z 'publicznej' kolejki,
// -klient wysyła do serwera 'publiczną' kolejką nazwę swojej 'prywatnej' kolejki/kolejek do komunikacji,
// -klient wysyła 'prywatną' kolejką nazwę katalogu,
// -w odpowiedzi serwer odsyła do klienta jego 'prywatną' kolejką zawartość tego katalogu (wynik polecenia ls),
// -klient wyświetla otrzymany wynik i działa dalej w pętli (może wysyłać kolejne nazwy katalogów), jeśli chce zakończyć połączenie wysyła do serwera pusty ciąg znaków i usuwa 'prywatną' kolejkę/kolejki.
// Uwaga: w systemie wydziałowym utworzenie kolejki funkcją mkfifo nie jest możliwe w katalogu domowym (brak uprawnień) ale działa poprawnie w katalogu /tmp


#include "connection.h"

#define null NULL
#define nullptr NULL

static volatile sig_atomic_t keep_running = 1;


typedef struct {
	char string[256];
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

void cleanAll() {
	for (int i = 0; i < queryArr.taken; i++) {
		if (access(queryArr.arr[i].string, F_OK) == 0) {
			unlink(queryArr.arr[i].string);
		}
	}
	freeArray(&queryArr);
}

static void sig_handler(int _) {
	printf("Client terminated.\n");
	(void)_;
	keep_running = 0;
	cleanAll();
	exit(EXIT_SUCCESS);
}



int main(int argc, char const* argv[]) {
	signal(SIGINT, sig_handler);
	srand(time(NULL));
	pid_t thisPid = getpid();
	// char hello[] = { '$', '\200', '\003' };
	// printf("%s\n", hello);
	String* myStr;

	initArray(&queryArr, 4);
	// String string = { "hello world" };
	char pfName[256];
	// pfName[0] = 'a';
	// pfName[1] = 'b';
	// pfName[2] = '\0';
	// String toDel;
	// strncpy(toDel.string, pfName, 20);
	// insertArray(&queryArr, toDel);
	// printf("%s\n", queryArr.queryArr[0]);
	int counter = 0;
	sleep(1);
	while (counter < 5) {
		int pubFifo = open(pubFifoName, O_WRONLY);
		if (pubFifo == -1) {
			printf("%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}

		int privateId = getRand(100000, 999999);
		char pfName[256];

		snprintf(pfName, sizeof(pfName), "%s%d-%dMFA%s", privateDir, thisPid, privateId, ".priv");

		String toDel;
		strncpy(toDel.string, pfName, 256);
		insertArray(&queryArr, toDel);

		printf("%s\n", pfName);

		// scanf("%s", &privFifo);

		mkfifo(pfName, 0777);
		char dirName[] = "/usr/";
		char data[3000];
		write(pubFifo, pfName, strlen(pfName));
		close(pubFifo);
		int privFifo = open(pfName, O_WRONLY);
		write(privFifo, dirName, strlen(dirName) + 1);
		close(privFifo);

		privFifo = open(pfName, O_RDONLY);
		read(privFifo, &data, 3000);
		close(privFifo);
		printf("%s\n", data);
		sleep(1);
		// unlink(pfName);
		counter++;
	}
	cleanAll();
	exit(EXIT_SUCCESS);
}