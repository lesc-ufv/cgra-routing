CSRC =c/main.c
CC=gcc
CFLAGS=-O2 -march=native
CLIB=
CBIN=bin/c

all:
	$(CC) $(CSRC) -o $(CBIN) $(CFLAGS) $(CLIB)

run:
	./$(CBIN)