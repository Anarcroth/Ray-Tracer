#include "Source.h"
#include "Vect.h"
#include "Color.h"
#include "Light.h"

Light::Light()
{
	position = Vect(0, 0, 0);
	color = Color(1, 1, 1, 0);
}

Light::Light(const Vect &p, const Color &c) : position(p), color(c)
{
}

Vect Light::getLightPosition() const { return position; }

Color Light::getLightColor() const { return color; }
