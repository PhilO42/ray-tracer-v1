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

	normal = myUtil::PosHom(1,1,1);
	color = myUtil::color(255,0,0);
}

Triangle::Triangle(MyVector _p0, MyVector _p1, MyVector _p2, MyVector _normal, MyVector _color) {
	p0 = _p0;
	p1 = _p1;
	p2 = _p2;
	center = p0+p1+p2;
	center *= 1.0/3.0;

	min = myUtil::color(std::min(std::min(p0(0),p1(0)),p2(0)),std::min(std::min(p0(1),p1(1)),p2(1)),std::min(std::min(p0(2),p1(2)),p2(2)));
	max = myUtil::color(std::max(std::max(p0(0),p1(0)),p2(0)),std::max(std::max(p0(1),p1(1)),p2(1)),std::max(std::max(p0(2),p1(2)),p2(2)));

	normal = _normal;
	color = _color;
//	cout << normal << center << p0	<< p1<<p2<<endl;
}

Triangle::~Triangle() {
	// TODO Auto-generated destructor stub
}

MyVector Triangle::collision(MyVector origin, MyVector direction, bool* collided, float* t_value, MyVector* collisionpoint, MyVector* _normal, bool isLightRay){
	direction = myUtil::normalize(direction);
	if((direction*normal) == 0){ //ray and normal orthogonal -> no hit
		*collided = false;
		cout << "90°" << endl;
		return myUtil::color(0,0,0);
	}

	MyVector e1 = p1-p0;
	MyVector e2 = p2-p0;
	MyVector s = origin - p0;

	//t
	MyVector cross = direction/e2;
	cross(3) = 0;//wegen homogenem crossproduct
	float normalizer = (1.0/(cross*e1));
	MyVector cross2 = s/e1;
	cross2(3) = 0;//wegen homogenem crossproduct
	*t_value = normalizer * (cross2 * e2);
	//b1
	float b1 = normalizer * (cross * s);
	//b2
	float b2 = normalizer * (cross2 * direction);

	if(b1+b2 > 1.0f || b1 < 0.0f || b2 < 0.0f || *t_value < myUtil::epsi){
		*collided = false;
		return myUtil::color(0,0,0);
	}
	*collided = true;
	*collisionpoint = origin + *t_value * direction;
	*_normal = normal;
	return color;
}

MyVector Triangle::getMin(){
//	cout << "min " << min << endl;
	return min;
}

MyVector Triangle::getMax(){
//	cout << "max " << max << endl;
	return max;
}

MyVector Triangle::getCenter(){
	return center;
}
