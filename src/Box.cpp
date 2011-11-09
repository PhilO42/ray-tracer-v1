/*
 * Box.cpp
 *
 *  Created on: 08.11.2011
 *      Author: philipp
 */

#include "Box.h"
#include "myUtil.h"

Box::Box(float _x0, float _x1, float _y0, float _y1, float _z0, float _z1, CVector<float> color) {
	boundingBox = AABB(_x0,_x1,_y0,_y1,_z0,_z1);
	col = color;
}

Box::~Box() {
	// TODO Auto-generated destructor stub
}

CVector<float> Box::collision(CVector<float> origin, CVector<float> direction, bool* collided, float* t_value, CVector<float>* collisionPoint, CVector<float>* normal, bool isLightRay){
	if(boundingBox.collision(origin,direction)){
		*collided = true;
		*t_value = 1;
		*collisionPoint = myUtil::PosHom(0,0,0);
		*normal = myUtil::PosHom(1,1,1);
		return col;
	}
	*collided = false;
	return CVector<float>(3,0);
}
