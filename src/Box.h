/*
 * Box.h
 *
 *  Created on: 08.11.2011
 *      Author: philipp
 */

#ifndef BOX_H_
#define BOX_H_

#include "SceneObject.h"
#include "AABB.h"

class Box : public SceneObject{
public:
	Box(float _x0, float _x1, float _y0, float _y1, float _z0, float _z1, CVector<float> color);
	virtual ~Box();
	CVector<float> collision(CVector<float> origin, CVector<float> direction, bool* collided, float* t_value, CVector<float>* collisionPoint, CVector<float>* normal, bool isLightRay);
private:
	AABB boundingBox;
	CVector<float> col;
};

#endif /* BOX_H_ */
