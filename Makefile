CC=gcc
CFLAGS=-fopenmp -O3

main: Pyra.o Solver.o Source.o
	$(CC) $(CFLAGS) -o Program Pyra.o Solver.o Source.o

clean:
	rm *.o
	rm Program
