#include "Pyra.h"
#include "Solver.h"
#include <omp.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <x86intrin.h>
#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
//#define DEBUG
//#define HARDDEBUG


//this function creates the association between integers of base N and moves, where N is the total number of moves that can be applied to the puzzle (inclusive of both CW and CCW rotations)
char* createAssociations(const char* sidesUsed)
{
	size_t len = sizeof(char) * strlen(sidesUsed) * 2 + 1;
	char* res = (char*)malloc(len);
	if (res == NULL)
		return NULL; //check for bad allocation
	
	for (char c = sidesUsed[0], i = 0; (unsigned int)i < strlen(sidesUsed); c = sidesUsed[(int)++i])
	{
		res[2 * i] = c; //set the nth element of res to the CW move
		res[2 * i + 1] = (char)toupper(c); //set the nth+1 element to the CCW move
	}

	res[2 * strlen(sidesUsed)] = 0; //null-terminate so we can do strlen later

	return res; //return the pointer
}

//converts the internal string used to represent an algorithm to a nice pretty formatted user-readable string
//this uses standard WCA algorithmic notation for pyraminxes
void outputSolution(const char* solution)
{
	int len = strlen(solution);
	//create a buffer
	const size_t size = sizeof(char) * len * 3 + 1;
	char outp[size];
	if (outp == NULL)
		return; //if the allocation failed, return
	outp[0] = 0; //set to null-terminated

	for (int i = 0; i < len; i++)
	{
		//convert internal moves to more human-readable moves
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
	
	//print the algorithm
	printf("Found solution: %s\n", outp);
}

//this converts a base N integer to a sequence of moves to apply to the pyraminx, based on the associations string
void convertMoves(char* numb, const char* associations)
{
	for (int i = 0; numb[i]; i++)
		numb[i] = associations[numb[i] - 48];
}

//checks if the puzzle is solved
bool solved(Puzzle* pyra)
{
	return pyra->f.top.color == pyra->f.left.color && //check if the front face is a solid color
		pyra->f.top.color == pyra->f.right.color &&
		pyra->f.top.color == pyra->f.left_e.color &&
		pyra->f.top.color == pyra->f.right_e.color &&
		pyra->f.top.color == pyra->f.bottom_e.color &&
		pyra->l.top.color == pyra->l.left.color && //check if the left face is a solid color
		pyra->l.top.color == pyra->l.right.color &&
		pyra->l.top.color == pyra->l.left_e.color &&
		pyra->l.top.color == pyra->l.right_e.color &&
		pyra->l.top.color == pyra->l.bottom_e.color &&
		pyra->r.top.color == pyra->r.left.color && //check if the right face is a solid color
		pyra->r.top.color == pyra->r.right.color &&
		pyra->r.top.color == pyra->r.left_e.color &&
		pyra->r.top.color == pyra->r.right_e.color &&
		pyra->r.top.color == pyra->r.bottom_e.color &&
		pyra->d.top.color == pyra->d.left.color && //check if the down face is a solid color
		pyra->d.top.color == pyra->d.right.color &&
		pyra->d.top.color == pyra->d.left_e.color &&
		pyra->d.top.color == pyra->d.right_e.color &&
		pyra->d.top.color == pyra->d.bottom_e.color;
}

//performs a solve based on a puzzle and a solve string
void performSolve(char* solvestr, Puzzle* temppyra)
{
	char* solvestrcopy = solvestr;
	//iterate through the solve string
	#pragma GCC unroll 20
	for (int i = 0; i < 100; i++)
	{
		//if we've run out of moves, early exit loop
		if (!*solvestrcopy) goto ps_exit;
		//perform the moves
		switch (*solvestrcopy)
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
		solvestrcopy++;
	}
	
ps_exit:
	//if solved, output solution in a human-readable format
	if (solved(temppyra))
		outputSolution(solvestr);
}

//converts an integer to a base N integer
//this works in 3 conditions:
//1. you don't mind the output being in reverse order (so 10 to base 8 is 21 not 12)
//2. you can guarantee that the base is <10
//3. input is positive
//I meet all of these conditions in this program, so this algorithm is very concise

//you are not expected to understand this, this has been highly optimized through hours of fine tuning
//this function alone is about the most efficient code I have ever written in my life
//uncommented
void convertBase(long long int numb, char* outp, int base, int* len)
{
	if (numb)
	{
		register int div, mod;
	
		switch(base)
		{
				int i;
			case 8:
				#pragma GCC unroll 22
				for (i = 0; i < 22; i++)
				{
					if (!numb) goto cb_8e;
					mod = numb & 7;
					numb >>= 3;
					outp[i] = mod | 48;
				}
				cb_8e: outp[i] = 0;
				*len = i;
				return;
			case 6:
				#pragma GCC unroll 25
				for (i = 0; i < 25; i++)
				{
					if (!numb) goto cb_6e;
					div = numb / 6;
					mod = numb - (div * 6);
					outp[i] = mod | 48;
					numb = div;
				}
				cb_6e: outp[i] = 0;
				*len = i;
				return;
			case 4:
				#pragma GCC unroll 32
				for (i = 0; i < 32; i++)
				{
					if (!numb) goto cb_4e;
					mod = numb & 3;
					numb >>= 2;
					outp[i] = mod | 48;
				}
				cb_4e: outp[i] = 0;
				*len = i;
				return;
			case 2:
				#pragma GCC unroll 64
				for (i = 0; i < 64; i++)
				{
					if (!numb) goto cb_2e;
					mod = numb & 1;
					numb >>= 1;
					outp[i] = mod | 48;
				}
				cb_2e: outp[i] = 0;
				*len = i;
				return;
		}
	}
	else
	{	
		outp[0] = '0';
		outp[1] = 0;
		return;
	}
}

//checks if an algorithm is redundant
bool isRedundant(const char* alg, int algLen)
{
	//iterate through the string
	for (int i = algLen - 2; i >= 0; i--)
	{
		//get difference of two moves
		int diff = alg[i] - alg[i + 1];
		//if they are the same move, or are inverses of each other, alg is redundant
		if (diff == 0 || diff == -32 || diff == 32)
			return true;
	}
	
	//alg is not redundant, calculate
	return false;
}

//solve the puzzle
void solvePuzzle(Puzzle* pyra, int maxMoves, const char* sidesUsed)
{
	//create the array of associations
	char* associations = createAssociations(sidesUsed);
	
	//spawn threads
	//we create a local copy everything so that we don't fight over memory
	#pragma omp parallel firstprivate(pyra, maxMoves, sidesUsed, associations)
	{
		long long int turn = (long long int)omp_get_thread_num(); //get the thread number. this will be used to calculate an algorithm
#ifdef DEBUG
		int oldlen = 0; //old algorithm length (for debugging purposes)
#endif
		
		//save the number of threads in a variable to avoid unnecessary function calls
		long long int num_threads = omp_get_num_threads();
		
		//precompute the base for later base conversions
		int base = strlen(sidesUsed) * 2;
		
		//convert the turn variable to an base N integer (where N is the number of possible turns we can make)
		char baseXstr[100];
		
		//create a temporary pyraminx to perform an algorithm on
		Puzzle temppyra;
		
		//cache this info so we don't have to compute it later
		char border = associations[1], replacement = associations[0];
		
		//we'll also be caching alg len
		int algLen = 0;
		
		//forever
		for (;;)
		{
			//copy the pyra
			temppyra = *pyra;
			
			//convert the int to a base N string
			convertBase(turn, baseXstr, base, &algLen);
			
#ifdef DEBUG
			//if we've moved from n-length algorithms to n+1-length algorithms, alert the user that this thread is done with n-length algorithms
			if (oldlen < algLen)
			{
				printf("Thread %d finished searching moves of length %d\n", omp_get_thread_num(), oldlen);
				oldlen = algLen;
			}
#endif
			
			//if we've exceeded maximum algorithm length, break
			if (algLen > maxMoves)
				break;
			
			//convert the base N string to a string of moves
			convertMoves(baseXstr, associations);
			
			//if the solve contains redundant moves like X X' or X X X, then ignore and continue
			if (!isRedundant(baseXstr, algLen))
			{
#ifdef HARDDEBUG
				//if we're SERIOUSLY debugging this program, output the algorithm we're trying
				printf("Trying algorithm: %s\n", baseXstr);
#endif
				
				//perform the solve
				performSolve(baseXstr, &temppyra);
				
				if (baseXstr[algLen - 1] != border)
				{
					//add the number of threads
					turn += num_threads;
					continue;
				}
				else
				{
					//this condition is because base N integers can NEVER start with a 0 (or end in a 0, since my base converter outputs mirrored)
					//because of this, algorithms can never end in certain moves (the move associated with the number 0)
					//this is a hack to make sure we're exhausting the totality of the solution space
					temppyra = *pyra;
					baseXstr[algLen - 1] = replacement;
					performSolve(baseXstr, &temppyra);
					
					//add the number of threads
					turn += num_threads;
					continue;
				}
			}
			else
			{
				//add the number of threads
				turn += num_threads;
				//continue searching
				continue;
			}
		}
	}

	//free the associations string
	free(associations);
}
