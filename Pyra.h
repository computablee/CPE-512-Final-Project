#pragma once
typedef enum
{
	Green, Blue, Yellow, Red, Blank
} Color;

typedef struct
{
	Color color;
} Edge;

typedef struct
{
	Color color;
} Center;

typedef struct
{
	Center top;
	Center left;
	Center right;
	Edge left_e;
	Edge right_e;
	Edge bottom_e;
} Side;

typedef struct
{
	Side f;
	Side l;
	Side r;
	Side d;
} Puzzle;


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
