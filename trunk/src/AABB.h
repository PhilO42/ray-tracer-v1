/*
 * AABB.h
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#ifndef AABB_H_
#define AABB_H_

#include "SceneObject.h"
#include "mathe/CVector.h"

class AABB {
public:
	AABB();
	AABB(float _x0, float _x1, float _y0, float _y1, float _z0, float _z1);
	virtual ~AABB();
	bool collision(CVector<float> origin, CVector<float> direction);
//	CVector<float> collision(CVector<float> origin, CVector<float> direction, bool* collided, float* t_value, CVector<float>* collisionpoint, CVector<float>* normal, CMatrix<float> cameraTransformation, CVector<float> bgColor, bool isLightRay);
private:
	float x0;
	float x1;
	float y0;
	float y1;
	float z0;
	float z1;
//	CVector<float> color;
};

#endif /* AABB_H_ */
