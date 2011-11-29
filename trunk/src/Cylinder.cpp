/*
 * Cylinder.cpp
 *
 *  Created on: 09.11.2011
 *      Author: philipp
 */

#include "Cylinder.h"
#include "myUtil.h"

Cylinder::Cylinder(CVector<float> _start, float _lenght, CVector<float> _color, float _r) {
	start = _start;
	length = _lenght;
	if(_color.size() == 9){
		color = _color;
	}else{
		color = myUtil::color9D(_color(0),_color(1),_color(2),_color(0),_color(1),_color(2),_color(0),_color(1),_color(2));
	}
	radius = _r;
	reflectionValue = 0.7;
	trancparencyValue = 0.0;
}

Cylinder::Cylinder(CVector<float> _start, float _lenght, CVector<float> _color, float _r, float trans, float refl) {
	start = _start;
	length = _lenght;
	if(_color.size() == 9){
		color = _color;
	}else{
		color = myUtil::color9D(_color(0),_color(1),_color(2),_color(0),_color(1),_color(2),_color(0),_color(1),_color(2));
	}
	radius = _r;
	reflectionValue = refl;
	trancparencyValue = trans;
}

Cylinder::~Cylinder() {
	// TODO Auto-generated destructor stub
}

CVector<float> Cylinder::collision(CVector<float> origin, CVector<float> direction, bool* collided, float* t_value, CVector<float>* collisionPoint, CVector<float>* normal, bool isLightRay, float* refl, float* trans){
	*refl = reflectionValue;
	*trans = trancparencyValue;
	float A = direction(0) * direction(0) + direction(2) * direction(2);
	float B = 2.0 * (direction(0) * (origin(0) - this->start(0))	+ direction(2) * (origin(2) - this->start(2)));
	float C = (origin(0) - this->start(0)) * (origin(0) - this->start(0))
			+ (origin(2) - this->start(2)) * (origin(2) - this->start(2))
			- this->radius * this->radius;

	if (((B * B - 4 * A * C) <= 0) || (A == 0)) {
		//Wurzel aus negativ
		//we dont hit the sphere
		*collided = false;
		*t_value = -1.0f;
		return myUtil::color(0, 0, 0);
	}
	//calculate t0,t1
	float t0 = (-B + sqrt(B * B - 4 * A * C)) / (2 * A);
	float t1 = (-B - sqrt(B * B - 4 * A * C)) / (2 * A);
	if ((t0 > myUtil::epsi) && (t1 > myUtil::epsi)) {
		*t_value = std::min(t0, t1);
		CVector<float> p0 = origin + direction * t0;
		if(((p0(1) - start(1))  > length) || (p0(1) < start(1))){
			*t_value = t1;
		}else{
			CVector<float> p1 = origin + direction * t1;
			if(((p1(1) - start(1))  > length) || (p1(1) < start(1))){
				*t_value = t0;
			}
		}
	} else {
		if ((t0 > myUtil::epsi) ^ (t1 > myUtil::epsi)) {
			*t_value = std::max(t0, t1);
		} else {
			//we dont hit the sphere
			*collided = false;
			*t_value = -1.0f;
			return myUtil::color(0, 0, 0);
		}
	}
	*collisionPoint = origin + direction * (*t_value);
	(*collisionPoint)(3) = 1.0;
	if((((*collisionPoint)(1) - start(1))  > length) || ((*collisionPoint)(1) < start(1))){
		*collided = false;
		return CVector<float>(3,0);
	}
	*normal = (*collisionPoint) - myUtil::PosHom(start(0),(*collisionPoint)(1),start(2));
	*normal = myUtil::normalize(*normal);
	*collided = true;
	return color;
}
