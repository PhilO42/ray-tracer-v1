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
	color = CVector<float>(9,140);
	min = myUtil::color(position(0)-radius,position(1)-radius,position(2)-radius);
	max = myUtil::color(position(0)+radius,position(1)+radius,position(2)+radius);
	reflectionValue = 0.0;
	trancparencyValue = 0.0;
}

Sphere::Sphere(float _radius, CVector<float> _position, CVector<float> _color, float refl, float trans){
	radius = _radius;
	position = _position;
	if(_color.size() == 9){
		color = _color;
	}else{
		color = myUtil::color9D(_color(0),_color(1),_color(2),_color(0),_color(1),_color(2),_color(0),_color(1),_color(2));
	}
	isLight =  false;
	min = myUtil::color(position(0)-radius,position(1)-radius,position(2)-radius);
	max = myUtil::color(position(0)+radius,position(1)+radius,position(2)+radius);
	reflectionValue = refl;
	trancparencyValue = trans;
}

Sphere::Sphere(float _radius, CVector<float> _position, CVector<float> _color, bool _isLight){
	radius = _radius;
	position = _position;
	if(_color.size() == 9){
		color = _color;
	}else{
		color = myUtil::color9D(_color(0),_color(1),_color(2),_color(0),_color(1),_color(2),_color(0),_color(1),_color(2));
	}
	isLight = _isLight;
	min = myUtil::color(position(0)-radius,position(1)-radius,position(2)-radius);
	max = myUtil::color(position(0)+radius,position(1)+radius,position(2)+radius);
	reflectionValue = 0.0;
	trancparencyValue = 0.0;
}

Sphere::~Sphere() {
	// TODO Auto-generated destructor stub
}

CVector<float> Sphere::collision(CVector<float> origin, CVector<float> direction, bool* collided, float* t_value, CVector<float>* collisionPoint, CVector<float>* normal, bool isLightRay, float* refl, float* trans){
	if(isLight && isLightRay){
		*collided = false;
//		*t_value = -1.0f;
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
	*trans = trancparencyValue;
	*refl = reflectionValue;
	*collided = true;
	*collisionPoint = origin + direction * (*t_value);
	(*collisionPoint)(3) = 1.0;
	*normal = (*collisionPoint) - this->position;
	*normal = myUtil::normalize(*normal);
	return this->color;
}

CVector<float> Sphere::getMin(){
	return min;
}

CVector<float> Sphere::getMax(){
	return max;
}

CVector<float> Sphere::getCenter(){
	return position;
}
