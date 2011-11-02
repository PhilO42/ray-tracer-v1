/*
 * Triangle.cpp
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#include "Triangle.h"
#include "myUtil.h"

Triangle::Triangle() {
	p0 = myUtil::PosHom(1,0,0);
	p1 = myUtil::PosHom(0,1,0);
	p2 = myUtil::PosHom(0,0,1);
	normal = myUtil::PosHom(1,1,1);
	color = myUtil::color(255,0,0);
}

Triangle::Triangle(CVector<float> _p0, CVector<float> _p1, CVector<float> _p2, CVector<float> _normal, CVector<float> _color) {
	p0 = _p0;
	p1 = _p1;
	p2 = _p2;
	normal = _normal;
	color = _color;
}

Triangle::~Triangle() {
	// TODO Auto-generated destructor stub
}

CVector<float> Triangle::collision(CVector<float> origin, CVector<float> direction, bool* collided, float* t_value, CVector<float>* collisionpoint, CVector<float>* _normal, bool isLightRay){
	if((direction*normal) == 0){ //ray and normal orthogonal -> no hit
		*collided = false;
		return myUtil::color(0,0,0);
	}

	CVector<float> e1 = p1-p0;
	CVector<float> e2 = p2-p0;
	CVector<float> s = origin - p0;

	//t
	CVector<float> cross = direction/e2;
	cross(3) = 0;//wegen homogenem crossproduct
	float normalizer = (1.0/(cross*e1));
	CVector<float> cross2 = s/e1;
	cross2(3) = 0;//wegen homogenem crossproduct
	*t_value = normalizer * (cross2 * e2);
	//b1
	float b1 = normalizer * (cross * s);
	//b2
	float b2 = normalizer * (cross2 * direction);

	if(b1+b2 > 1.0f || b1 < 0.0f || b2 < 0.0f){
		*collided = false;
		return myUtil::color(0,0,0);
	}
	*collided = true;
	*collisionpoint = origin + *t_value * direction;
	*_normal = normal;
	return color;
}