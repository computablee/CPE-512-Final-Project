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
	Center top __attribute__ ((aligned(64)));
	Center left __attribute__ ((aligned(64)));
	Center right __attribute__ ((aligned(64)));
	Edge left_e __attribute__ ((aligned(64)));
	Edge right_e __attribute__ ((aligned(64)));
	Edge bottom_e __attribute__ ((aligned(64)));
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
inline void rotateEdges(Edge* edge1, Edge* edge2, Edge* edge3);
inline void rotateCenters(Center* center1, Center* center2, Center* center3);
void l(Puzzle* pyra);
void lp(Puzzle* pyra);
void u(Puzzle* pyra);
void up(Puzzle* pyra);
void r(Puzzle* pyra);
void rp(Puzzle* pyra);
void b(Puzzle* pyra);
void bp(Puzzle* pyra);
