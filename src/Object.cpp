#include "Ray.h"
#include "Vect.h"
#include "Color.h"
#include "Object.h"

Color Object::getColor() const { return Color(0.0, 0.0, 0.0, 0); }

Vect Object::getNormalAt(Vect &intersection_position) const { return Vect(0, 0, 0); }

double Object::findIntersection(Ray ray)
{
	return 0;
}
