#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define PAGE_SIZE 256

int main () {
	int fd;
	fd = open("test.dat", O_RDWR);
	char data[PAGE_SIZE];
	memset(data, 'A', PAGE_SIZE);
	if (write(fd, data, PAGE_SIZE) == -1) {
		printf("fail...");
	}
}


