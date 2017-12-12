#ifndef _PLANE_H
#define _PLANE_H

#include "math.h"
#include "Object.h"
#include "Vect.h"
#include "Color.h"

class Plane : public Object
{
private:
	Vect normal;
	double distance;
	Color color;

public:
	Plane();

	Plane(const Vect &, const double &, const Color &);

	Vect getPlaneNormal() const;
	double getPlaneDistance() const;
  
	virtual Color getColor() const;
	virtual Vect getNormalAt(Vect point) const;
	virtual double findIntersection(Ray ray);
};

#endif
