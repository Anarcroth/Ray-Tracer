#ifndef _RAY_H
#define _RAY_H

#include "Vect.h"

class Ray {

	Vect origin, direction;

public:

	Ray();

	Ray(const Vect &, const Vect &);

	//method functions

	Vect getRayOrigin();
	Vect getRayDirection();

};

#endif
