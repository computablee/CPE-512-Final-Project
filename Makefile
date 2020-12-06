CC=clang
CFLAGS=-fopenmp -Ofast -funroll-loops -Werror -Wall -march=haswell

main: Pyra.o Solver.o Source.o
	$(CC) $(CFLAGS) -o Program Pyra.o Solver.o Source.o

clean:
	rm *.o
	rm Program
