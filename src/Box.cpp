/*
 * Box.cpp
 *
 *  Created on: 08.11.2011
 *      Author: philipp
 */

#include "Box.h"
#include "myUtil.h"
#include <limits>
#include <iostream>

using namespace std;

Box::Box(CVector<float> center, CVector<float> _sizes, CVector<float> color) {
	_sizes *= 0.5;
	//CVector<float> p0, CVector<float> p1, CVector<float> p2, CVector<float> _normal, CVector<float> _color
	CVector<float> A = myUtil::PosHom(center(0) + _sizes(0), center(1) - _sizes(1), center(2) - _sizes(2));
	CVector<float> B = myUtil::PosHom(center(0) + _sizes(0), center(1) + _sizes(1), center(2) - _sizes(2));
	CVector<float> C = myUtil::PosHom(center(0) + _sizes(0), center(1) - _sizes(1), center(2) + _sizes(2));
	CVector<float> D = myUtil::PosHom(center(0) + _sizes(0), center(1) + _sizes(1), center(2) + _sizes(2));
	CVector<float> E = myUtil::PosHom(center(0) - _sizes(0), center(1) - _sizes(1), center(2) - _sizes(2));
	CVector<float> F = myUtil::PosHom(center(0) - _sizes(0), center(1) + _sizes(1), center(2) - _sizes(2));
	CVector<float> G = myUtil::PosHom(center(0) - _sizes(0), center(1) - _sizes(1), center(2) + _sizes(2));
	CVector<float> H = myUtil::PosHom(center(0) - _sizes(0), center(1) + _sizes(1), center(2) + _sizes(2));
	planes = vector< Plane* >(6);
	planes.at(0) = (new Plane(B,D,F,myUtil::PosHom(0,1,0),myUtil::color(255,255,255)));//top
	planes.at(1) = (new Plane(A,C,E,myUtil::PosHom(0,-1,0),myUtil::color(0,255,0)));//bottom

	planes.at(2) = (new Plane(A,B,C,myUtil::PosHom(-1,0,0),myUtil::color(0,0,255)));//left
	planes.at(3) = (new Plane(E,G,F,myUtil::PosHom(1,0,0),myUtil::color(255,0,0)));//right

	planes.at(4) = (new Plane(A,E,B,myUtil::PosHom(0,0,-1),myUtil::color(0,255,255)));//back
	planes.at(5) = (new Plane(G,C,H,myUtil::PosHom(0,0,1),myUtil::color(0,0,0)));//front
}

Box::~Box() {
	// TODO Auto-generated destructor stub
}

CVector<float> Box::collision(CVector<float> origin, CVector<float> direction, bool* collided, float* t_value, CVector<float>* collisionPoint, CVector<float>* normal, bool isLightRay){
	*collided = false;
	float t_val = numeric_limits<float>::max();
	float t_val_last = numeric_limits<float>::max();
	CVector<float> colPnt;
	CVector<float> norm;
	CVector<float> color_best(3,0);
	for(int i = 0; i < planes.size(); i++){
		bool col = false;
		CVector<float> color = planes[i]->collision(origin,direction,&col,&t_val,&colPnt,&norm,isLightRay);
		if(col){
			*collided = true;
			if(t_val < t_val_last){
				t_val_last = t_val;
				*t_value = t_val;
				*collisionPoint = colPnt;
				*normal = norm;
				color_best = color;
			}
		}
	}
	return color_best;
}
