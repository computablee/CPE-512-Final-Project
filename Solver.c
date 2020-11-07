#include "Pyra.h"
#include "Solver.h"
#include <omp.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
//#define DEBUG
//#define HARDDEBUG

char* createAssociations(const char* sidesUsed)
{
	size_t len = sizeof(char) * strlen(sidesUsed) * 2 + 1;
	char* res = (char*)malloc(len);
	if (res == NULL)
		return NULL;
	
	for (char c = sidesUsed[0], i = 0; (unsigned int)i < strlen(sidesUsed); c = sidesUsed[(int)++i])
	{
		res[2 * i] = c;
		res[2 * i + 1] = (char)toupper(c);
	}

	res[2 * strlen(sidesUsed)] = 0;

	return res;
}

void convertMoves(char* numb, const char* associations)
{
	for (int i = 0; (unsigned int)i < strlen(numb); i++)
	{
		char chr[2] = { numb[i], 0 };
		numb[i] = associations[atoi(chr)];
	}
}

bool solved(Puzzle pyra)
{
	return pyra.f.top.color == pyra.f.left.color &&
		pyra.f.top.color == pyra.f.right.color &&
		pyra.f.top.color == pyra.f.left_e.color &&
		pyra.f.top.color == pyra.f.right_e.color &&
		pyra.f.top.color == pyra.f.bottom_e.color &&
		pyra.l.top.color == pyra.l.left.color &&
		pyra.l.top.color == pyra.l.right.color &&
		pyra.l.top.color == pyra.l.left_e.color &&
		pyra.l.top.color == pyra.l.right_e.color &&
		pyra.l.top.color == pyra.l.bottom_e.color &&
		pyra.r.top.color == pyra.r.left.color &&
		pyra.r.top.color == pyra.r.right.color &&
		pyra.r.top.color == pyra.r.left_e.color &&
		pyra.r.top.color == pyra.r.right_e.color &&
		pyra.r.top.color == pyra.r.bottom_e.color &&
		pyra.d.top.color == pyra.d.left.color &&
		pyra.d.top.color == pyra.d.right.color &&
		pyra.d.top.color == pyra.d.left_e.color &&
		pyra.d.top.color == pyra.d.right_e.color &&
		pyra.d.top.color == pyra.d.bottom_e.color;
}

void outputSolution(const char* solution)
{
	const size_t size = sizeof(char) * strlen(solution) * 3 + 1;
	char* outp = (char*)malloc(size);
	if (outp == NULL)
		return;
	outp[0] = 0;

	for (int i = 0; (unsigned int)i < strlen(solution); i++)
	{
		switch(solution[i])
		{
		case 'l':
			strcat(outp, "L ");
			break;
		case 'L':
			strcat(outp, "L' ");
			break;
		case 'u':
			strcat(outp, "U ");
			break;
		case 'U':
			strcat(outp, "U' ");
			break;
		case 'r':
			strcat(outp, "R ");
			break;
		case 'R':
			strcat(outp, "R' ");
			break;
		case 'b':
			strcat(outp, "B ");
			break;
		case 'B':
			strcat(outp, "B' ");
			break;
		default:
			break;
		}
	}
	printf("Found solution: %s\n", outp);
	free(outp);
}

inline void performSolve(char* solvestr, Puzzle* temppyra)
{
	for (int i = 0; (unsigned int)i < strlen(solvestr); i++)
	{
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

	if (solved(*temppyra))
		outputSolution(solvestr);
}

void convertBase(int numb, char* outp, int base)
{
	int pos = 0;
	if (numb == 0)
	{
		outp[0] = '0';
		outp[1] = 0;
		return;
	}
	while (numb > 0)
	{
		sprintf(&outp[pos], "%d", numb % base);
		numb -= numb % base;
		numb /= base;
		pos++;
	}
	outp[pos] = 0;
}

void solvePuzzle(Puzzle pyra, int maxMoves, const char* sidesUsed)
{
	char* associations = createAssociations(sidesUsed);
	
#pragma omp parallel firstprivate(pyra) shared(maxMoves, sidesUsed, associations)
	{
		bool in_length = true;
		int turn = omp_get_thread_num();
		int oldlen = 0;
		while (in_length)
		{
			Puzzle temppyra = pyra;
			char baseXstr[100];
			convertBase(turn, baseXstr, (int)strlen(sidesUsed) * 2);
			
#ifdef DEBUG
			if (oldlen < (int)strlen(baseXstr))
			{
				printf("Thread %d finished searching moves of length %d\n", omp_get_thread_num(), oldlen);
				oldlen = (int)strlen(baseXstr);
			}
#endif
			if (strlen(baseXstr) > (size_t)maxMoves)
			{
				in_length = false;
				continue;
			}
			
			convertMoves(baseXstr, associations);

#ifdef HARDDEBUG
			printf("Trying algorithm: %s\n", baseXstr);
#endif

			performSolve(baseXstr, &temppyra);
			if (baseXstr[strlen(baseXstr) - 1] == 'L')
			{
				temppyra = pyra;
				baseXstr[strlen(baseXstr) - 1] = 'l';
				performSolve(baseXstr, &temppyra);
			}

			turn += omp_get_num_threads();
		}
	}

	free(associations);
}
