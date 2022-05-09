#include <errno.h>
const char pubFifoName[] = "/tmp/serverMFA.pub";
const char appName[] = "MFA";
const char privateExt[] = ".priv";
const char privatePath[] = "/tmp/";

static int rv;
static fd_set readCheck;
static fd_set writeCheck;
static fd_set errCheck;
static struct timeval timeout;

int prepareWrite(const char* name) {
	int fd = open(name, O_WRONLY);
	FD_SET(fd, &readCheck);
	FD_SET(fd, &errCheck);
	FD_SET(fd, &writeCheck);
	// timeout.tv_sec = 1;
	// timeout.tv_usec = 0;
	rv = select(fd + 1, &readCheck, &writeCheck, &errCheck, NULL);
	if (rv < 0) {
			// printf("Select failed\n");
		printf("Connection lost: select failed.\n");
		return -1;
	}

	if (FD_ISSET(fd, &errCheck)) {
		printf("Connection lost: FD error\n");
		return -1;
	}

	if (fd == -1) {
		printf("Connection lost.\n");
		return -1;
	}
	return fd;
}

int readTo(const char* name, char* buffer) {
	int fd = open(name, O_RDONLY);
	FD_SET(fd, &readCheck);
	FD_SET(fd, &errCheck);
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	rv = select(fd + 1, &readCheck, NULL, &errCheck, &timeout);

	int x = 0;
	char pfName[PATH_MAX + 1];

	if (fd == -1) {
		return -1;
	}

	if (rv < 0) {
		printf("Select failed\r\n");
		return -1;
	}

	if (FD_ISSET(fd, &errCheck)) {
		printf("FD error\r\n");
		return -1;
	}

	if (FD_ISSET(fd, &readCheck)) {
		memset(buffer, 0, sizeof(buffer));
		rv = read(fd, buffer, sizeof(buffer));
		if (rv < 0) {
			printf("Read failed\r\n");
			return -1;
		}
		printf(buffer);

		if (rv == 0) {
			close(fd);
			return -2;
		}
		else {
			buffer[rv] = '\0';
		}
	}
	close(fd);
	return 0;
}
