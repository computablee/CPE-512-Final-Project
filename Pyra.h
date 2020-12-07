#pragma once
//enum of colors
typedef enum
{
	Green, Blue, Yellow, Red, Blank
} Color;

//type of edge
typedef struct
{
	Color color;
} Edge;

//type of center
typedef struct
{
	Color color;
} Center;

//type of side (3 centers, 3 edges)
typedef struct
{
	Center top;
	Center left;
	Center right;
	Edge left_e;
	Edge right_e;
	Edge bottom_e;
} Side;

//type of puzzle (4 sides)
typedef struct
{
	Side f;
	Side l;
	Side r;
	Side d;
} Puzzle;

//see Pyra.c for explanations of all of these functions
void rotateEdges(Edge* edge1, Edge* edge2, Edge* edge3);
void rotateCenters(Center* center1, Center* center2, Center* center3);
void l(Puzzle* pyra);
void lp(Puzzle* pyra);
void u(Puzzle* pyra);
void up(Puzzle* pyra);
void r(Puzzle* pyra);
void rp(Puzzle* pyra);
void b(Puzzle* pyra);
void bp(Puzzle* pyra);
