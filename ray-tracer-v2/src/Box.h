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
#include <string>

class Box : public SceneObject{
public:
	Box(CVector<float> center, CVector<float> _sizes, CVector<float> color, float trans, float refl, std::string path);
	virtual ~Box();
	CVector<float> collision(CVector<float> origin, CVector<float> direction, bool* collided, float* t_value, CVector<float>* collisionPoint, CVector<float>* normal, bool isLightRay, float* refl, float* trans);
	void rotate(CVector<float> angles);
private:
	std::vector< Plane* > planes;
};

#endif /* BOX_H_ */
