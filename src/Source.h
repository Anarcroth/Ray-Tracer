#ifndef _SOURCE_H
#define _SOURCE_H

#include "Ray.h"
#include "Vect.h"
#include "Color.h"

class Source
{
public:
	virtual Vect getLightPosition() const;
	virtual Color getLightColor() const;

};

#endif
