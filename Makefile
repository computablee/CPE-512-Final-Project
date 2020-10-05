CC=gcc
CFLAGS=-fopenacc -fopenmp -O3 -pedantic-errors -Werror -Wall
CPFLAGS=-lm

main: main.o
	$(CC) $(CFLAGS) -o main main.o $(CPFLAGS)