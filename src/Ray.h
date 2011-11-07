/*
 * Ray.h
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#ifndef RAY_H_
#define RAY_H_

#include "MyVector.h"

class Ray {
public:
	Ray(MyVector _origin, MyVector _direction);
	virtual ~Ray();
	//MyVector getColor(SceneGraph* graph);
	MyVector origin;
	MyVector direction;
private:
	bool calculatedColor;
	MyVector color;
};

#endif /* RAY_H_ */
