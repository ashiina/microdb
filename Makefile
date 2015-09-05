
CC = cc
CFLAGS = -g

all: microdb all-test
all-test: test-file

do-test: test-file
	./test-file

microdb: 
test-file: test-file.o file.o
	$(CC) -o test-file $(CFLAGS) test-file.o file.o

file.o: file.c microdb.h
	$(CC) -o file.o $(CFLAGS) -c file.c

test-file.o: test-file.c microdb.h
	$(CC) -o test-file.o $(CFLAGS) -c test-file.c

clean: 
	rm -f *.o

