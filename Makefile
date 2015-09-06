
CC = cc
CFLAGS = -g

all: clean microdb all-test

all-test: test-file test-datadef

do-test: test-file test-datadef
	./test-file 
	./test-datadef

microdb: 

test-file: test-file.o file.o
	$(CC) -o test-file $(CFLAGS) test-file.o file.o
test-datadef: test-datadef.o datadef.o
	$(CC) -o test-datadef $(CFLAGS) test-datadef.o datadef.o file.o
file.o: file.c microdb.h
	$(CC) -o file.o $(CFLAGS) -c file.c
datadef.o: datadef.c microdb.h 
	$(CC) -o datadef.o $(CFLAGS) -c datadef.c
test-file.o: test-file.c microdb.h
	$(CC) -o test-file.o $(CFLAGS) -c test-file.c
test-datadef.o: test-datadef.c microdb.h
	$(CC) -o test-datadef.o $(CFLAGS) -c test-datadef.c

clean: 
	rm -f *.o

