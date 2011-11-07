/*
 * SceneObject.h
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#ifndef SCENEOBJECT_H_
#define SCENEOBJECT_H_

#include "MyVector.h"
#include <vector>

class SceneObject {
public:
	SceneObject(){};
	virtual ~SceneObject(){};
	virtual MyVector collision(MyVector origin, MyVector direction, bool* collided, float* t_value, MyVector* collisionPoint, MyVector* normal, bool isLightRay){};
	virtual MyVector getMin(){};
	virtual MyVector getMax(){};
	virtual MyVector getCenter(){};
};

#endif /* SCENEOBJECT_H_ */
