/*
 * Cylinder.h
 *
 *  Created on: 09.11.2011
 *      Author: philipp
 */

#ifndef CYLINDER_H_
#define CYLINDER_H_

#include "SceneObject.h"
#include "mathe/CVector.h"

class Cylinder: public SceneObject {
public:
	Cylinder(CVector<float> _start, float _lenght, CVector<float> _color, float _r);
	virtual ~Cylinder();
	CVector<float> collision(CVector<float> origin, CVector<float> direction, bool* collided, float* t_value, CVector<float>* collisionPoint, CVector<float>* normal, bool isLightRay);
private:
	CVector<float> start;
	CVector<float> color;
	float length;
	float radius;
};

#endif /* CYLINDER_H_ */
