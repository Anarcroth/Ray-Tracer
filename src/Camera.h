#ifndef _CAMERA_H
#define _CAMERA_H

#include "Vect.h"

class Camera
{
 private:
	//parameters used to define the general coordinates for the scene
	//used in method functions in the added vectors
	//compute the perspectives
	Vect campos, camdir, camright, camdown;

 public:

	Camera();

	Camera(const Vect &, const Vect &, const Vect &, const Vect &);

	Vect getCameraPosition();
	Vect getCameraDirection();
	Vect getCameraRight();
	Vect getCameraDown();

};

#endif
