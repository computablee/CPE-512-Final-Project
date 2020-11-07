#include "Pyra.h"
#include "Solver.h"
#include <stdio.h>
#include <ctype.h>

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

int main()
{
	Side front;
	Side left;
	Side right;
	Side bottom;
	
	printf("Welcome to the Pyraminx solver.\n");

	getSide(&front, "front");
	getSide(&left, "left");
	getSide(&right, "right");
	getSide(&bottom, "bottom");

	Puzzle pyra = { .f = front, .l = left, .r = right, .d = bottom };
	int maxMoves;
	char sidesUsed[5], tryagain;
	do
	{
		printf("Enter the maximum number of moves for the algorithm:\n");
		scanf("%d", &maxMoves);

		printf("Enter the sides allowed for the algorithm [lrub]:\n");
		scanf("%s", sidesUsed);

		solvePuzzle(pyra, maxMoves, sidesUsed);

		printf("Generated all algorithms.\n");

		printf("Try again? (y/n)\n");
		scanf(" %c", &tryagain);
	} while (tryagain == 'y');

	char garbage;
	scanf("%c", &garbage);
	return 0;
}
