#include "Vect.h"
#include "Ray.h"

Ray::Ray() {
	origin = Vect(0, 0, 0);
	direction = Vect(1, 0, 0);
}

Ray::Ray(const Vect &o, const Vect &d) {
	origin = o;
	direction = d;
}

Vect Ray::getRayOrigin()
{
	return origin;
}

Vect Ray::getRayDirection()
{
	return direction;
}
