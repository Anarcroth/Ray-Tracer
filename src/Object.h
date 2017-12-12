#ifndef _OBJECT_H
#define _OBJECT_H

#include "Ray.h"
#include "Vect.h"
#include "Color.h"

class Object
{
public:
	virtual Color getColor() const;

	virtual Vect getNormalAt(Vect &intersection_position) const;

	virtual double findIntersection(Ray ray);

};

#endif
