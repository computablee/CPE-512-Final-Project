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

//this converts a base N integer to a sequence of moves to apply to the pyraminx, based on the associations string
void convertMoves(char* numb, const char* associations)
{
	for (int i = 0; (unsigned int)i < strlen(numb); i++)
	{
		char chr[2] = { numb[i], 0 }; //create a string containing the single character of the place in *numb to convert
		numb[i] = associations[atoi(chr)]; //convert to a move based on associations (note: itoa is not in gcc, but atoi is??)
	}
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

//converts the internal string used to represent an algorithm to a nice pretty formatted user-readable string
//this uses standard WCA algorithmic notation for pyraminxes
void outputSolution(const char* solution)
{
	//create a buffer
	const size_t size = sizeof(char) * strlen(solution) * 3 + 1;
	char* outp = (char*)malloc(size);
	if (outp == NULL)
		return; //if the allocation failed, return
	outp[0] = 0; //set to null-terminated

	for (int i = 0; (unsigned int)i < strlen(solution); i++)
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
	free(outp); //free
}

//converts an integer to a base N integer
//this works in 3 conditions:
//1. you don't mind the output being in reverse order (so 10 to base 8 is 21 not 12)
//2. you can guarantee that the base is <10
//3. input is positive
//I meet all of these conditions in this program, so this algorithm is very concise
//comments omitted because I feel it's a relatively self-explanatory base conversion function that needs no further explanation
void convertBase(long long int numb, char* outp, int base)
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
		outp[pos] = (numb % base) + 48;
		numb -= numb % base;
		numb /= base;
		pos++;
	}
	outp[pos] = 0;
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
		bool in_length = true; //boolean to check if we're under or equal to the maximum algorithm length
		long long int turn = (long long int)omp_get_thread_num(); //get the thread number. this will be used to calculate an algorithm
		int oldlen = 0; //old algorithm length (for debugging purposes)
		
		//while we're under or equal to maximum algorithm length
		while (in_length)
		{
			//create a temporary pyraminx to perform an algorithm on
			Puzzle temppyra = *pyra;
			//convert the turn variable to an base N integer (where N is the number of possible turns we can make)
			char baseXstr[100];
			convertBase(turn, baseXstr, (int)strlen(sidesUsed) * 2);
			
#ifdef DEBUG
			//if we've moved from n-length algorithms to n+1-length algorithms, alert the user that this thread is done with n-length algorithms
			if (oldlen < (int)strlen(baseXstr))
			{
				printf("Thread %d finished searching moves of length %d\n", omp_get_thread_num(), oldlen);
				oldlen = (int)strlen(baseXstr);
			}
#endif
			//if we've exceeded maximum algorithm length, break
			//this use of a flag/continue is to avoid a goto
			//while I honestly would much prefer a goto, I've had professors in the past criticize me for even very conservative goto use
			//want to avoid the risk of getting counted off
			if (strlen(baseXstr) > (size_t)maxMoves)
			{
				in_length = false;
				continue;
			}
			
			//convert the base N string to a string of moves
			convertMoves(baseXstr, associations);

#ifdef HARDDEBUG
			//if we're SERIOUSLY debugging this program, output the algorithm we're trying
			printf("Trying algorithm: %s\n", baseXstr);
#endif

			//perform the solve
			performSolve(baseXstr, &temppyra);
			//this condition is because base N integers can NEVER start with a 0 (or end in a 0, since my base converter outputs mirrored)
			//because of this, algorithms can never end in certain moves (the move associated with the number 0)
			//this is a hack to make sure we're exhausting the totality of the solution space
			if (baseXstr[strlen(baseXstr) - 1] == associations[1])
			{
				temppyra = *pyra;
				baseXstr[strlen(baseXstr) - 1] = associations[0];
				performSolve(baseXstr, &temppyra);
			}

			//add the number of threads
			turn += (long long int)omp_get_num_threads();
		}
	}

	//free the associations string
	free(associations);
}
