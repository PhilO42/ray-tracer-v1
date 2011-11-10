/*
 * Plane.cpp
 *
 *  Created on: 09.11.2011
 *      Author: philipp
 */

#include "Plane.h"
#include "myUtil.h"
#include <QColor>

Plane::Plane(CVector<float> _p0, CVector<float> _p1, CVector<float> _p2, CVector<float> _normal, CVector<float> _color, float trans, float refl) {
	p0 = _p0;
	p1 = _p1;
	p2 = _p2;
	CVector<float> a = (p1-p0);
	a *= (1.0/2.0);
	CVector<float> b = (p2-p0);
	b *= (1.0/2.0);
	center = p0 + a + b;
	color = _color;
	normal = myUtil::normalize(_normal);
	xSize = myUtil::homogenNorm(p1-p0);
	ySize = myUtil::homogenNorm(p2-p0);
	image.load("wood.jpg");

	reflectionValue = trans;
	trancparencyValue = refl;
}

Plane::~Plane() {
	// TODO Auto-generated destructor stub
}
CVector<float> Plane::collision(CVector<float> origin, CVector<float> direction, bool* collided, float* t_value, CVector<float>* collisionpoint, CVector<float>* _normal, bool isLightRay, float* refl, float* trans){
	direction = myUtil::normalize(direction);
		if((direction*normal) == 0){ //ray and normal orthogonal -> no hit
			*collided = false;
//			cout << "90°" << endl;
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

		if (b1 > 1.0f || b2 > 1.0f || b1 < 0.0f || b2 < 0.0f || *t_value < myUtil::epsi) {
			*collided = false;
			return CVector<float>(3,0);
		}
		*collided = true;
		*trans = trancparencyValue;
		*refl = reflectionValue;
		*collisionpoint = origin + *t_value * direction;
		*_normal = normal;
		return getColor(b1,b2);
}

CVector<float> Plane::getColor(float x, float y){
	QRgb rgb = image.pixel(min((int)(x*image.width()),image.width()-1),min((int)(y*image.height()),image.height()-1));
	return myUtil::color(qRed(rgb),qGreen(rgb),qBlue(rgb));
}
