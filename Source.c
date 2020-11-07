#include "Pyra.h"
#include "Solver.h"
#include <stdio.h>
#include <ctype.h>

//this function gets a user input and returns a color
Color getColor()
{
	char c;
	scanf(" %c", &c);
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

//main
int main()
{
	Side front;
	Side left;
	Side right;
	Side bottom;
	
	printf("Welcome to the Pyraminx solver.\n");

	//get inputs of all of the sides
	getSide(&front, "front");
	getSide(&left, "left");
	getSide(&right, "right");
	getSide(&bottom, "bottom");

	//create the puzzle and some variables for max moves and sides used
	Puzzle pyra = { .f = front, .l = left, .r = right, .d = bottom };
	int maxMoves;
	char sidesUsed[5], tryagain;
	
	do
	{
		//get the maximum move count to include in the search
		printf("Enter the maximum number of moves for the algorithm:\n");
		scanf("%d", &maxMoves);

		//get the sides allowed to turn in the algorithm (input of 'l' permits both L and L' as moves, so this is really just getting if we're doing a 2-gen, 3-gen, or 4-gen algorithm)
		printf("Enter the sides allowed for the algorithm [lrub]:\n");
		scanf("%s", sidesUsed);

		//generate the algorithms
		solvePuzzle(&pyra, maxMoves, sidesUsed);

		//alert that we've generated all the algorithms and prompt the user to try again
		printf("Generated all algorithms.\n");

		printf("Try again? (y/n)\n");
		scanf(" %c", &tryagain);
	} while (tryagain == 'y');
	
	return 0;
}
