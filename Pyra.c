#include "Pyra.h"

//performs an L
void l(Puzzle* pyra)
{
	rotateEdges(&pyra->f.left_e, &pyra->l.bottom_e, &pyra->d.right_e);
	rotateEdges(&pyra->f.bottom_e, &pyra->l.right_e, &pyra->d.left_e);
	rotateCenters(&pyra->f.left, &pyra->l.right, &pyra->d.top);
}

//performs an L'
void lp(Puzzle* pyra)
{
	rotateEdges(&pyra->l.bottom_e, &pyra->f.left_e, &pyra->d.right_e);
	rotateEdges(&pyra->l.right_e, &pyra->f.bottom_e, &pyra->d.left_e);
	rotateCenters(&pyra->l.right, &pyra->f.left, &pyra->d.top);
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
	rotateEdges(&pyra->r.left_e, &pyra->f.left_e, &pyra->l.left_e);
	rotateEdges(&pyra->r.right_e, &pyra->f.right_e, &pyra->l.right_e);
	rotateCenters(&pyra->r.top, &pyra->f.top, &pyra->l.top);
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
	rotateEdges(&pyra->f.bottom_e, &pyra->r.left_e, &pyra->d.bottom_e);
	rotateEdges(&pyra->f.right_e, &pyra->r.bottom_e, &pyra->d.right_e);
	rotateCenters(&pyra->f.right, &pyra->r.left, &pyra->d.right);
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
	rotateEdges(&pyra->r.bottom_e, &pyra->l.left_e, &pyra->d.left_e);
	rotateEdges(&pyra->r.right_e, &pyra->l.bottom_e, &pyra->d.bottom_e);
	rotateCenters(&pyra->r.right, &pyra->l.left, &pyra->d.left);
}