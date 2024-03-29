/*
 * Plane.cpp
 *
 *  Created on: 09.11.2011
 *      Author: philipp
 */

#include "Plane.h"
#include "myUtil.h"
#include <QColor>
#include "SceneGraph.h"

Plane::Plane(CVector<float> _p0, CVector<float> _p1, CVector<float> _p2, CVector<float> _normal, CVector<float> _color, float trans, float refl, std::string path) {
	p0 = _p0;
	p1 = _p1;
	p2 = _p2;
	CVector<float> a = (p1-p0);
	a *= (1.0/2.0);
	CVector<float> b = (p2-p0);
	b *= (1.0/2.0);
	center = p0 + a + b;
	if(_color.size() == 9){
		color = _color;
	}else{
		color = myUtil::color9D(_color(0),_color(1),_color(2),_color(0),_color(1),_color(2),_color(0),_color(1),_color(2));
	}
	normal = myUtil::normalize(_normal);
	xSize = myUtil::homogenNorm(p1-p0);
	ySize = myUtil::homogenNorm(p2-p0);
	image.load(QString(path.c_str()));
	reflectionValue = trans;
	trancparencyValue = refl;
	bumpy = false;
}

Plane::Plane(CVector<float> _p0, CVector<float> _p1, CVector<float> _p2, CVector<float> _normal, CVector<float> _color, float trans, float refl, std::string texture, std::string _bumpmap) {
	p0 = _p0;
	p1 = _p1;
	p2 = _p2;
	CVector<float> a = (p1-p0);
	a *= (1.0/2.0);
	CVector<float> b = (p2-p0);
	b *= (1.0/2.0);
	center = p0 + a + b;
	if(_color.size() == 9){
		color = _color;
	}else{
		color = myUtil::color9D(_color(0),_color(1),_color(2),_color(0),_color(1),_color(2),_color(0),_color(1),_color(2));
	}
	normal = myUtil::normalize(_normal);
	xSize = myUtil::homogenNorm(p1-p0);
	ySize = myUtil::homogenNorm(p2-p0);
	image.load(QString(texture.c_str()));
	bumpMap.load(QString(_bumpmap.c_str()));
	reflectionValue = trans;
	trancparencyValue = refl;
	bumpy = true;
}

Plane::~Plane() {
	// TODO Auto-generated destructor stub
}
CVector<float> Plane::collision(CVector<float> origin, CVector<float> direction, bool* collided, float* t_value, CVector<float>* collisionpoint, CVector<float>* _normal, bool isLightRay, float* refl, float* trans){
	direction = myUtil::normalize(direction);
//		if((direction*normal) == 0){ //ray and normal orthogonal -> no hit
//			*collided = false;
//			cout << "90°" << endl;
//			return CVector<float>(3,0);
//		}

		CVector<float> e1 = p1-p0;
		CVector<float> e2 = p2-p0;
		CVector<float> s = origin - p0;

		//t
		CVector<float> cross = direction/e2;
		cross = myUtil::PosHom(cross(0),cross(1),cross(2),0);
//		cross(3) = 0;//wegen homogenem crossproduct
		float normalizer = (1.0/(cross*e1));
		CVector<float> cross2 = s/e1;
		cross = myUtil::PosHom(cross(0),cross(1),cross(2),0);
//		cross2(3) = 0;//wegen homogenem crossproduct
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
		*_normal = getNormal(b1,b2);
		return getColor(b1,b2);
}

CVector<float> Plane::getColor(float x, float y){
	if(x != x)
		x = 0;
	if(y != y)
		y = 0;
	if(x < 0)
		x = 0;
	if(y < 0)
		y = 0;
	if(x > image.width()-1)
		x = image.width()-1;
	if(y > image.height()-1)
		y = image.height()-1;
	QRgb rgb = image.pixel(min((int)(x*image.width()),image.width()-1),min((int)(y*image.height()),image.height()-1));
	return myUtil::color9D(qRed(rgb),qGreen(rgb),qBlue(rgb),qRed(rgb),qGreen(rgb),qBlue(rgb),qRed(rgb),qGreen(rgb),qBlue(rgb));
}

CVector<float> Plane::getNormal(float x, float y){
	if(bumpy){
		if(x != x)
			x = 0;
		if(y != y)
			y = 0;
		int pixelPosX = min((int)(x*bumpMap.width()),bumpMap.width()-1);
		int pixelPosY = min((int)(y*bumpMap.height()),bumpMap.height()-1);
		float center = qRed(bumpMap.pixel(pixelPosX, pixelPosY));
		float xMinus = qRed(bumpMap.pixel(max(0,pixelPosX-1), pixelPosY));
		float xPlus = qRed(bumpMap.pixel(min(pixelPosX+1,bumpMap.width()-1), pixelPosY));
		float yMinus = qRed(bumpMap.pixel(pixelPosX, max(0,pixelPosY-1)));
		float yPlus = qRed(bumpMap.pixel(pixelPosX, min(pixelPosY+1,bumpMap.height()-1)));

		float x = (xPlus-center)-(xMinus-center);
		float z = (yPlus-center)-(yMinus-center);
		CVector<float> back = myUtil::color(-x,30,-z);
		back += normal;
		back *= (1.0/back.norm());
		return myUtil::PosHom(back(0),back(1),back(2));
	}else{
		return normal;
	}
}

void Plane::rotate(CVector<float> angles){
	CMatrix<float> Mat = SceneGraph::Rx(angles(0))*SceneGraph::Ry(angles(1))*SceneGraph::Rz(angles(2));
	p0 -= center;
	p0 = Mat*p0;
	p0 += center;
	p1 -= center;
	p1 = Mat*p1;
	p1 += center;
	p2 -= center;
	p2 = Mat*p2;
	p2 += center;
	normal = Mat * normal;
}
