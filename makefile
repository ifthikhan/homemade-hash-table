CC=gcc
CFLAGS=-c -Wall

all: demo

demo: hash-table.o main.o
	$(CC) hash-table.o main.o -o demo

tests: hash-table.o hash-table-tests.o
	$(CC) hash-table.o hash-table-tests.o -o tests

tests.o: hash-table-tests.c
	$(CC) $(CFLAGS) hash-table-tests.c

main.o: main.c
	$(CC) $(CFLAGS) main.c

hash-table.o: hash-table.c
	$(CC) $(CFLAGS) hash-table.c

clean:
	rm -rf *o demo tests
