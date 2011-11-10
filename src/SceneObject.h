/*
 * SceneObject.h
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#ifndef SCENEOBJECT_H_
#define SCENEOBJECT_H_

#include "mathe/CVector.h"
#include <vector>
#include "mathe/CMatrix.h"

class SceneObject {
public:
	SceneObject(){};
	virtual ~SceneObject(){};
	virtual CVector<float> collision(CVector<float> origin, CVector<float> direction, bool* collided, float* t_value, CVector<float>* collisionPoint, CVector<float>* normal, bool isLightRay, float* refl, float* trans){};
	virtual CVector<float> getMin(){};
	virtual CVector<float> getMax(){};
	virtual CVector<float> getCenter(){};
	float reflectionValue;
	float trancparencyValue;
};

#endif /* SCENEOBJECT_H_ */
