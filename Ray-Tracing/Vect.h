#ifndef _VECT_H
#define _VECT_H

#include "math.h"

class Vect {

private:

	//the X, Y, Z cooridnates in the plane
	double x, y, z;

public:

	Vect();

	Vect(const double &, const double &, const double &);

	//method functions

	double getVectX();
	double getVectY();
	double getVectZ();

	double magnitude();

	Vect normalize();

	Vect negative();

	double dotProduct(Vect &v);

	Vect crossProduct(Vect &v);

	Vect vectAdd(Vect &v);

	Vect vectMult(double scalar);
};

#endif
