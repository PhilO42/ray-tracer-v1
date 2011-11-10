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
#include "Plane.h"
#include <vector>

class Box : public SceneObject{
public:
	Box(CVector<float> center, CVector<float> _sizes, CVector<float> color, float trans=0, float refl=0.7);
	virtual ~Box();
	CVector<float> collision(CVector<float> origin, CVector<float> direction, bool* collided, float* t_value, CVector<float>* collisionPoint, CVector<float>* normal, bool isLightRay, float* refl, float* trans);
private:
	std::vector< Plane* > planes;
};

#endif /* BOX_H_ */
