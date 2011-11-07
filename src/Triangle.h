/*
 * Triangle.h
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "SceneObject.h"
#include "MyVector.h"

class Triangle : public SceneObject {
public:
	Triangle();
	Triangle(MyVector _p0, MyVector _p1, MyVector _p2, MyVector _normal, MyVector _color);
	virtual ~Triangle();
	MyVector collision(MyVector origin, MyVector direction, bool* collided, float* t_value, MyVector* collisionpoint, MyVector* _normal, bool isLightRay);
	MyVector getMin();
	MyVector getMax();
	MyVector getCenter();
private:
	//corners
	MyVector p0;
	MyVector p1;
	MyVector p2;
	//normal & color
	MyVector normal;
	MyVector color;
	MyVector min;
	MyVector max;
	MyVector center;
};

#endif /* TRIANGLE_H_ */
