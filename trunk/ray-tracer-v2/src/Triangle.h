/*
 * Triangle.h
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "SceneObject.h"
#include "mathe/CVector.h"

class Triangle : public SceneObject {
public:
	Triangle();
	Triangle(CVector<float> _p0, CVector<float> _p1, CVector<float> _p2, CVector<float> _normalp0, CVector<float> _normalp1, CVector<float> _normalp2, CVector<float> _color);
	virtual ~Triangle();
	CVector<float> collision(CVector<float> origin, CVector<float> direction, bool* collided, float* t_value, CVector<float>* collisionpoint, CVector<float>* _normal, bool isLightRay, float* refl, float* trans);
	CVector<float> getMin();
	CVector<float> getMax();
	CVector<float> getCenter();
	void rotate(CVector<float> angles, CVector<float> _center);
	void translate(CVector<float> movement);
private:
	//corners
	CVector<float> p0;
	CVector<float> p1;
	CVector<float> p2;
	//normal & color
	CVector<float> normal;
	CVector<float> normalp0;
	CVector<float> normalp1;
	CVector<float> normalp2;
	CVector<float> color;
	CVector<float> min;
	CVector<float> max;
	CVector<float> center;
};

#endif /* TRIANGLE_H_ */
