#include "math.h"
#include "Object.h"
#include "Vect.h"
#include "Color.h"
#include "Plane.h"

Plane::Plane()
{
	normal = Vect(1, 0, 0);
	distance = 0;
	color = Color(0.5, 0.5, 0.5, 0);
}

Plane::Plane(const Vect &normalValue, const double &distanceValue, const Color &colorValue) : normal(normalValue), distance(distanceValue), color(colorValue)
{
}

Vect Plane::getPlaneNormal() const { return normal; }
double Plane::getPlaneDistance() const { return distance; }
Color Plane::getColor() const { return color; }
Vect Plane::getNormalAt(Vect point) const {	return normal; }

double Plane::findIntersection(Ray ray)
{
	Vect ray_direction = ray.getRayDirection();

	//'a' is the dot product between the direction of the ray is coming from and the normal vector of the plane
 	double a = ray_direction.dotProduct(normal);

	if (a == 0)
    {
      //ray is parallel to the plane
      return -1;
    }
	else
    {
      double b = normal.dotProduct(ray.getRayOrigin().vectAdd(normal.vectMult(distance).negative()));
      //the distance from the ray origin to the intersection
      return -1 * b / a;
    }
}
