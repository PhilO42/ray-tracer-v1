/*
 * AABB.cpp
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#include "AABB.h"
#include <iostream>

AABB::AABB() {
	float x0 = -1;
	float x1 = 1;
	float y0 = -1;
	float y1 = 1;
	float z0 = -1;
	float z1 = 1;
}

AABB::AABB(float _x0, float _x1, float _y0, float _y1, float _z0, float _z1){
	float x0 = _x0;
	float x1 = _x1;
	float y0 = _y0;
	float y1 = _y1;
	float z0 = _z0;
	float z1 = _z1;
	std::cout << "x: " << _x0 << " -> " << _x1 << " y: " << _y0 << " -> " << _y1 << " z: " << _z0 << " -> " << _z1 << std::endl;
}

AABB::~AABB() {
	// TODO Auto-generated destructor stub
}

bool AABB::collision(MyVector origin, MyVector direction){
	float txmin;
	float txmax;
	float tymin;
	float tymax;
	float tzmin;
	float tzmax;
	float t0 = (x0 - origin(0))/direction(0);
	float t1 = (x1 - origin(0))/direction(0);
	if(t0 < t1){
		txmin = t0;
		txmax = t1;
	}else{
		txmin = t1;
		txmax = t0;
	}

	t0 = (y0 - origin(1))/direction(1);
	t1 = (y1 - origin(1))/direction(1);
	if(t0 < t1){
		tymin = t0;
		tymax = t1;
	}else{
		tymin = t1;
		tymax = t0;
	}

	t0 = (z0 - origin(2))/direction(2);
	t1 = (z1 - origin(2))/direction(2);
	if(t0 < t1){
		tzmin = t0;
		tzmax = t1;
	}else{
		tzmin = t1;
		tzmax = t0;
	}

	if(((txmax < tymin)||(tymax < txmin)) && ((txmax < tzmin)||(tzmax < txmin)) && ((tymax < tzmin)||(tzmax < tymin))){//AABB || BBAA
		return false;
	}
	return true;
}
