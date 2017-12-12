#include "Ray.h"
#include "Vect.h"
#include "Color.h"
#include "Source.h"

Vect Source::getLightPosition() const { return Vect(0, 0, 0); }
Color Source::getLightColor() const { return Color(1, 1, 1, 0); }
