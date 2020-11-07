#include "Pyra.h"

//performs a rotation of 3 edges
inline void rotateEdges(Edge* edge1, Edge* edge2, Edge* edge3)
{
	const Edge temp = *edge1;
	*edge1 = *edge2;
	*edge2 = *edge3;
	*edge3 = temp;
}

//performs a rotation of 3 centers
inline void rotateCenters(Center* center1, Center* center2, Center* center3)
{
	const Center temp = *center1;
	*center1 = *center2;
	*center2 = *center3;
	*center3 = temp;
}

//performs an L
void l(Puzzle* pyra)
{
	rotateEdges(&pyra->f.left_e, &pyra->l.bottom_e, &pyra->d.right_e);
	rotateEdges(&pyra->f.bottom_e, &pyra->l.right_e, &pyra->d.left_e);
	rotateCenters(&pyra->f.left, &pyra->l.right, &pyra->d.top);
}

//performs an L'
//NOTE: could make this more efficient by not calling l(Puzzle*) twice but instead actually performing a *real* L'
//however, it is time consuming for me to define rotations, so I'm being lazy and doing this instead
//this note omitted in other X' move functions
void lp(Puzzle* pyra)
{
	l(pyra); l(pyra);
}

//performs a U
void u(Puzzle* pyra)
{
	rotateEdges(&pyra->f.left_e, &pyra->r.left_e, &pyra->l.left_e);
	rotateEdges(&pyra->f.right_e, &pyra->r.right_e, &pyra->l.right_e);
	rotateCenters(&pyra->f.top, &pyra->r.top, &pyra->l.top);
}

//performs a U'
void up(Puzzle* pyra)
{
	u(pyra); u(pyra);
}

//performs an R
void r(Puzzle* pyra)
{
	rotateEdges(&pyra->r.left_e, &pyra->f.bottom_e, &pyra->d.bottom_e);
	rotateEdges(&pyra->r.bottom_e, &pyra->f.right_e, &pyra->d.right_e);
	rotateCenters(&pyra->r.left, &pyra->f.right, &pyra->d.right);
}

//performs an R'
void rp(Puzzle* pyra)
{
	r(pyra); r(pyra);
}

//performs a B
void b(Puzzle* pyra)
{
	rotateEdges(&pyra->l.left_e, &pyra->r.bottom_e, &pyra->d.left_e);
	rotateEdges(&pyra->l.bottom_e, &pyra->r.right_e, &pyra->d.bottom_e);
	rotateCenters(&pyra->l.left, &pyra->r.right, &pyra->d.left);
}

//performs a B'
void bp(Puzzle* pyra)
{
	b(pyra); b(pyra);
}