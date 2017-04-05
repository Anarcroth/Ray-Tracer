#ifndef _PLANE_H
#define _PLANE_H

#include "math.h"
#include "Object.h"
#include "Vect.h"
#include "Color.h"

class Plane : public Object {

	Vect normal;
	double distance;
	Color color;

public:

	Plane();

	Plane(const Vect &, const double &, const Color &);

	//method functions

	Vect getPlaneNormal();
	double getPlaneDistance();
	virtual Color getColor();

	virtual Vect getNormalAt(Vect point);

	virtual double findIntersection(Ray ray);

};

#endif
