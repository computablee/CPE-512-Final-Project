#include "Pyra.h"
#include "Solver.h"
#include <stdio.h>
#include <ctype.h>
#include <omp.h>
#include <stdbool.h>

//this function gets a user input and returns a color
Color getColor()
{
	char c;
	
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wunused-result"
	scanf(" %c", &c);
	#pragma GCC diagnostic pop
	
	c = (char)tolower((int)c);

	if (c == 'r')
		return Red;
	if (c == 'g')
		return Green;
	if (c == 'y')
		return Yellow;
	if (c == 'b')
		return Blue;
	
	return Blank;
}

//this function gets the color inputs of a side
void getSide(Side* side, const char* sideStr)
{
	printf("Enter the top center of %s:\n", sideStr);
	side->top.color = getColor();

	printf("Enter the left center of %s:\n", sideStr);
	side->left.color = getColor();

	printf("Enter the right center of %s:\n", sideStr);
	side->right.color = getColor();

	printf("Enter the left edge of %s:\n", sideStr);
	side->left_e.color = getColor();

	printf("Enter the right edge of %s:\n", sideStr);
	side->right_e.color = getColor();

	printf("Enter the bottom edge of %s:\n", sideStr);
	side->bottom_e.color = getColor();
}

//this function converts a scramble into a puzzle start position
Puzzle getScramble()
{
	printf("Terminate scramble with a space followed by a '!'\n");
	Puzzle pyra = { .f = (Side) { .top = (Center) { .color = Green }, .left = (Center) { .color = Green }, .right = (Center) { .color = Green }, .left_e = (Edge) { .color = Green }, .right_e = (Edge) { .color = Green }, .bottom_e = (Edge) { .color = Green } },
					 .l = (Side) { .top = (Center) { .color = Blue }, .left = (Center) { .color = Blue }, .right = (Center) { .color = Blue }, .left_e = (Edge) { .color = Blue }, .right_e = (Edge) { .color = Blue }, .bottom_e = (Edge) { .color = Blue } },
					 .r = (Side) { .top = (Center) { .color = Red }, .left = (Center) { .color = Red }, .right = (Center) { .color = Red }, .left_e = (Edge) { .color = Red }, .right_e = (Edge) { .color = Red }, .bottom_e = (Edge) { .color = Red } },
					 .d = (Side) { .top = (Center) { .color = Yellow }, .left = (Center) { .color = Yellow }, .right = (Center) { .color = Yellow }, .left_e = (Edge) { .color = Yellow }, .right_e = (Edge) { .color = Yellow }, .bottom_e = (Edge) { .color = Yellow } } };
		
	printf("Input scramble:\n");
	char move[3];
	char internalMove[2] = { 0 };
	
	while (true)
	{
		
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wunused-result"
		scanf("%s", move);
		#pragma GCC diagnostic pop
		if (move[0] == '!')
			return pyra;
		
		switch (move[0])
		{
			case 'L': case 'l':
				internalMove[0] = 'l';
				break;
			case 'U': case 'u':
				internalMove[0] = 'u';
				break;
			case 'R': case 'r':
				internalMove[0] = 'r';
				break;
			case 'B': case 'b':
				internalMove[0] = 'b';
				break;
			default:
				fprintf(stderr, "Error, move not recognized: %c", move[0]);
				return pyra;
		}
		
		if (move[1] == '\'')
			internalMove[0] = (char)toupper(internalMove[0]);
		
		performSolve(internalMove, &pyra);
	}
}

//main
int main()
{
	Side front;
	Side left;
	Side right;
	Side bottom;
	char inputMethod;
	Puzzle pyra;
	
	printf("Welcome to the Pyraminx solver.\n");

	printf("Input (S)cramble or (P)ermutation?\n");
	
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wunused-result"
	scanf("%c", &inputMethod);
	
	if (inputMethod == 'P' || inputMethod == 'p')
	{
		//get inputs of all of the sides
		getSide(&front, "front");
		getSide(&left, "left");
		getSide(&right, "right");
		getSide(&bottom, "bottom");
		
		pyra = (Puzzle) { .f = front, .l = left, .r = right, .d = bottom };
	}
	else
	{
		pyra = getScramble();
	}

	//create the puzzle and some variables for max moves and sides used
	int maxMoves, maxThreads;
	char sidesUsed[5], tryagain;
	
	do
	{
		//get the maximum move count to include in the search
		printf("Enter the maximum number of moves for the algorithm:\n");
		scanf("%d", &maxMoves);

		//get the sides allowed to turn in the algorithm (input of 'l' permits both L and L' as moves, so this is really just getting if we're doing a 2-gen, 3-gen, or 4-gen algorithm)
		printf("Enter the sides allowed for the algorithm [lrub]:\n");
		scanf("%s", sidesUsed);
		
		//get the max number of threads
		printf("Enter the maximum number of threads allowed to be used by the algorithm finder (or enter 0 to automatically use the maximum):\n");
		scanf("%d", &maxThreads);
		
		if (maxThreads != 0)
			omp_set_num_threads(maxThreads);

		//generate the algorithms
		solvePuzzle(&pyra, maxMoves, sidesUsed);

		//alert that we've generated all the algorithms and prompt the user to try again
		printf("Generated all algorithms.\n");

		printf("Try again? (y/n)\n");
		scanf(" %c", &tryagain);
		#pragma GCC diagnostic pop
	} while (tryagain == 'y');
	
	return 0;
}
