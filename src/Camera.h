#ifndef _CAMERA_H
#define _CAMERA_H

#include "Vect.h"

class Camera
{
 private:
	Vect campos, camdir, camright, camdown;

 public:

	Camera();

	Camera(const Vect &, const Vect &, const Vect &, const Vect &);

	Vect getCameraPosition() const;
	Vect getCameraDirection() const;
	Vect getCameraRight() const;
	Vect getCameraDown() const;

};

#endif
