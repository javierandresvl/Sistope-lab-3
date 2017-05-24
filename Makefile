CC=gcc
route=src/

all: main.o functions.o
	$(CC) main.o functions.o -o lab3 -Wall -I.
	rm main.o functions.o

main.o: $(route)main.c
	$(CC) -c $(route)main.c

functions.o: $(route)functions.c
	$(CC) -c $(route)functions.c