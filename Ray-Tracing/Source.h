#ifndef _SOURCE_H
#define _SOURCE_H

#include "Ray.h"
#include "Vect.h"
#include "Color.h"

class Source {

public:

	Source();

	//method functions
	virtual Vect getLightPosition();
	virtual Color getLightColor();

};

#endif
