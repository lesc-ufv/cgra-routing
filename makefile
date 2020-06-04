CSRC =c/main.c c/fsm/common.c c/fsm/SimpleWriteOnExec.c c/fsm/BacktrackWriteOnExec.c
CC=gcc
CFLAGS=-O2 -march=native -Wall -Wextra -Wno-unused-result
CLIB=
CBIN=bin/c

all:
	$(CC) $(CSRC) -o $(CBIN) $(CFLAGS) $(CLIB)

debug:
	$(CC) $(CSRC) -o $(CBIN) $(CFLAGS) $(CLIB) -DDEBUG

run:
	./$(CBIN)

test:
	./$(CBIN) benchmarks/sbcci/grid/verilog_chebyshev.out benchmarks/sbcci/edge_list/chebyshev/chebyshev_0.in output/sbcci/chebyshev_0.csv