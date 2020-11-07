#pragma once
#include "Pyra.h"
#include <stdbool.h>

//see Solver.c for descriptions
void solvePuzzle(Puzzle pyra, int maxMoves, const char* sidesUsed);
char* createAssociations(const char* sidesUsed);
void convertMoves(char* numb, const char* associations);
bool solved(Puzzle pyra);
void outputSolution(const char* solution);
inline void performSolve(char* solvestr, Puzzle* pyra);
void convertBase(int numb, char* outp, int base);