/*
 * Sphere.cpp
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#include "Sphere.h"
#include "math.h"
#include "myUtil.h"

using namespace std;


Sphere::Sphere() {
	radius = 1.0;
	position = CVector<float>(4,1);
	color = CVector<float>(3,140);
}

Sphere::Sphere(float _radius, CVector<float> _position, CVector<float> _color){
	radius = _radius;
	position = _position;
	color = _color;
	isLight =  false;
}

Sphere::Sphere(float _radius, CVector<float> _position, CVector<float> _color, bool _isLight){
	radius = _radius;
	position = _position;
	color = _color;
	isLight = _isLight;
}

Sphere::~Sphere() {
	// TODO Auto-generated destructor stub
}

CVector<float> Sphere::collision(CVector<float> origin, CVector<float> direction, bool* collided, float* t_value, CVector<float>* collisionPoint, CVector<float>* normal, bool isLightRay){
	if(isLight && isLightRay){
		*collided = false;
		*t_value = -1.0f;
		return myUtil::color(0,0,0);
	}
	CVector<float> pos;
	pos = position;

	float A = direction(0) * direction(0)
			+ direction(1) * direction(1)
			+ direction(2) * direction(2);
	float B = 2.0 * (direction(0) * (origin(0) - this->position(0))
			+ direction(1) * (origin(1) - this->position(1))
			+ direction(2) * (origin(2) - this->position(2)));
	float C = (origin(0) - this->position(0)) * (origin(0) - this->position(0))
			+ (origin(1) - this->position(1)) * (origin(1) - this->position(1))
			+ (origin(2) - this->position(2)) * (origin(2) - this->position(2)) - this->radius * this->radius;

	if(((B*B - 4*A*C) <= 0) || (A == 0)){
		//Wurzel aus negativ
		//we dont hit the sphere
		*collided = false;
		*t_value = -1.0f;
		return myUtil::color(0,0,0);
	}
	//calculate t0,t1
	float t0 = (-B+sqrt(B*B - 4*A*C))/(2*A);
	float t1 = (-B-sqrt(B*B - 4*A*C))/(2*A);
	if((t0 > myUtil::epsi) && (t1 > myUtil::epsi)){
		*t_value = std::min(t0,t1);
	}else{
		if((t0 > myUtil::epsi) ^ (t1 > myUtil::epsi)){
		*t_value = std::max(t0,t1);
		}else{
			//we dont hit the sphere
			*collided = false;
			*t_value = -1.0f;
			return myUtil::color(0,0,0);
		}
	}
	*collided = true;
	*collisionPoint = origin + direction * (*t_value);
	(*collisionPoint)(3) = 1.0;
	*normal = (*collisionPoint) - this->position;
	*normal = myUtil::normalize(*normal);
	return this->color;
}
