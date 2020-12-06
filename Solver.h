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
inline void performSolve(char* solvestr, Puzzle* pyra);
void convertBase(long long int numb, char* outp, int base);
bool isRedundant(const char* baseXstr);

//performs a solve based on a puzzle and a solve string
inline void performSolve(char* solvestr, Puzzle* temppyra)
{
	//iterate through the solve string
	for (int i = 0; (unsigned int)i < strlen(solvestr); i++)
	{
		//perform the moves
		switch (solvestr[i])
		{
		case 'l':
			l(temppyra);
			break;
		case 'L':
			lp(temppyra);
			break;
		case 'u':
			u(temppyra);
			break;
		case 'U':
			up(temppyra);
			break;
		case 'r':
			r(temppyra);
			break;
		case 'R':
			rp(temppyra);
			break;
		case 'b':
			b(temppyra);
			break;
		case 'B':
			bp(temppyra);
			break;
		default:
			break;
		}
	}

	//if solved, output solution in a human-readable format
	if (solved(temppyra))
		outputSolution(solvestr);
}