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
	center = p0+p1+p2;
	center *= 1.0/3.0;

	min = myUtil::color(0,0,0);
	max = myUtil::color(1,1,1);

	normalp0 = myUtil::PosHom(1,1,1);
	normalp1 = myUtil::PosHom(1,1,1);
	normalp2 = myUtil::PosHom(1,1,1);
	normal = normalp0 + normalp1 + normalp2;
	normal *= (1.0/3.0);
	color = myUtil::color(255,0,0);

	reflectionValue = 0.3;
	trancparencyValue = 0.3;
}

Triangle::Triangle(CVector<float> _p0, CVector<float> _p1, CVector<float> _p2, CVector<float> _normalp0, CVector<float> _normalp1, CVector<float> _normalp2, CVector<float> _color) {
	p0 = _p0;
	p1 = _p1;
	p2 = _p2;
	center = p0+p1+p2;
	center *= 1.0/3.0;
	min = myUtil::color(std::min(std::min(p0(0),p1(0)),p2(0)),std::min(std::min(p0(1),p1(1)),p2(1)),std::min(std::min(p0(2),p1(2)),p2(2)));
	max = myUtil::color(std::max(std::max(p0(0),p1(0)),p2(0)),std::max(std::max(p0(1),p1(1)),p2(1)),std::max(std::max(p0(2),p1(2)),p2(2)));
	normalp0 = _normalp0;
	normalp1 = _normalp1;
	normalp2 = _normalp2;
	normal = normalp0 + normalp1 + normalp2;
	normal *= (1.0/3.0);
	color = _color;
//	cout << normal << center << p0	<< p1<<p2<<endl;

	reflectionValue = 0.3;
	trancparencyValue = 0.3;
}

Triangle::~Triangle() {
	// TODO Auto-generated destructor stub
}

CVector<float> Triangle::collision(CVector<float> origin, CVector<float> direction, bool* collided, float* t_value, CVector<float>* collisionpoint, CVector<float>* _normal, bool isLightRay, float* refl, float* trans){
	direction = myUtil::normalize(direction);
	if((direction*normal) == 0){ //ray and normal orthogonal -> no hit
		*collided = false;
		cout << "90°" << endl;
		return CVector<float>(3,0);
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

	if(b1+b2 > 1.0f || b1 < 0.0f || b2 < 0.0f || *t_value < myUtil::epsi){
		*collided = false;
		return CVector<float>(3,0);
	}
	*collided = true;
	*refl = reflectionValue;
	*trans = trancparencyValue;
	*collisionpoint = origin + *t_value * direction;
	CVector<float> a = normalp0;
//	a *= -1.0;
//	if(myUtil::homogenNorm(*collisionpoint - p0) != 0)
		a *= 1.0/myUtil::homogenNorm(*collisionpoint - p0);
	CVector<float> b = normalp1;
//	b *= -1.0;
//	if(myUtil::homogenNorm(*collisionpoint - p1) != 0)
		b *= 1.0/myUtil::homogenNorm(*collisionpoint - p1);
	CVector<float> c = normalp2;
//	c *= -1.0;
//	if(myUtil::homogenNorm(*collisionpoint - p2) != 0)
		c *= 1.0/myUtil::homogenNorm(*collisionpoint - p2);

	*_normal = myUtil::normalize(a+b+c);//a+b+c
//	*_normal = CVector<float>(4,1);
	return color;
}

CVector<float> Triangle::getMin(){
	return min;
}

CVector<float> Triangle::getMax(){
	return max;
}

CVector<float> Triangle::getCenter(){
	return center;
}
