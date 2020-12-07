#pragma once
#include "Pyra.h"
#include <stdbool.h>
#include <string.h>

//see Solver.c for descriptions
void solvePuzzle(Puzzle* pyra, int maxMoves, const char* sidesUsed);
char* createAssociations(const char* sidesUsed);
void convertMoves(char* numb, const char* associations);
bool solved(Puzzle* pyra);
void outputSolution(const char* solution);
void convertBase(long long int numb, char* outp, int base, int* len);
bool isRedundant(const char* alg, int algLen);
void performSolve(char* solvestr, Puzzle* temppyra);
