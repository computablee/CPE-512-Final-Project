CC=gcc
CFLAGS=-fopenmp -Ofast -funroll-loops -Werror -Wall -fPIE -march=haswell

main: Pyra.o Solver.o Source.o
	$(CC) $(CFLAGS) -o Program Pyra.o Solver.o Source.o

solver: Solver.c
	$(CC) $(CFLAGS) -S -masm=intel -o Solver.s Solver.c

clean:
	rm *.o
	rm Program
