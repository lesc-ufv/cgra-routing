CSRC =c/main.c c/fsm/common.c
CC=gcc
CFLAGS=-O2 -march=native -Wall -Wextra -Wno-unused-result
CLIB=
CBIN=bin/c

all:
	$(CC) $(CSRC) -o $(CBIN) $(CFLAGS) $(CLIB)

run:
	./$(CBIN)