/*
 * Plane.h
 *
 *  Created on: 09.11.2011
 *      Author: philipp
 */

#ifndef PLANE_H_
#define PLANE_H_

#include "mathe/CVector.h"
#include "SceneObject.h"
#include <QImage>

class Plane : public SceneObject{
//p1
//p0 p2
public:
	Plane(CVector<float> p0, CVector<float> p1, CVector<float> p2, CVector<float> _normal, CVector<float> _color);
	virtual ~Plane();
	CVector<float> collision(CVector<float> origin, CVector<float> direction, bool* collided, float* t_value, CVector<float>* collisionPoint, CVector<float>* normal, bool isLightRay);
private:
	CVector<float> p0;
	CVector<float> p1;
	CVector<float> p2;
	CVector<float> color;
	CVector<float> center;
	CVector<float> normal;
	float xSize;
	float ySize;
	QImage image;
	CVector<float> getColor(float x, float y);
};

#endif /* PLANE_H_ */