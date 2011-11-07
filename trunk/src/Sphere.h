/*
 * Sphere.h
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include "SceneObject.h"
#include "MyVector.h"

class Sphere : public SceneObject{
public:
	Sphere();
	Sphere(float _radius, MyVector _position, MyVector _color);
	Sphere(float _radius, MyVector _position, MyVector _color, bool _isLight);
	virtual ~Sphere();
	MyVector collision(MyVector origin, MyVector direction, bool* collided, float* t_value, MyVector* collisionpoint, MyVector* normal, bool isLightRay);
	MyVector getMin();
	MyVector getMax();
	MyVector getCenter();
private:
	float radius;
	MyVector position;
	MyVector color;
	bool isLight;
	MyVector min;
	MyVector max;
};

#endif /* SPHERE_H_ */
