CFLAGS=-g
CC=gcc
LIBS=-pthread
PROG=main

all: $(PROG)

main:
	$(CC) $(CFLAGS) -o main main.c $(LIBS)

clean:
	rm -f *.o $(PROG)

