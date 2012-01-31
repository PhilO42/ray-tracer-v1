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
	textured = false;
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
	textured = false;
}

Sphere::Sphere(float _radius, CVector<float> _position, CVector<float> _color, float refl, float trans, string path){
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
	image.load(QString(path.c_str()));
	textured = true;
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
	textured = false;
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
	return getColor(*collisionPoint);
	//return this->color;
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

void Sphere::rotate(CVector<float> angles){

}

CVector<float> Sphere::getColor(CVector<float> collisionPoint){//in globalen werten
	if(!textured)
		return color;

	//http://www.cs.unc.edu/~rademach/xroads-RT/RTarticle.html
	CVector<float> Vn = myUtil::color(0,1,0);
	CVector<float> Ve = myUtil::color(1,0,0);
	CVector<float> Vp = myUtil::color(collisionPoint(0) - position(0),collisionPoint(1) - position(1),collisionPoint(2) - position(2));
	Vp = myUtil::normalize(collisionPoint - position);

    float phi = acos(-(Vn*Vp));
    float v = phi/myUtil::PI;
    float u;
    float theta = (acos((Vp*Ve) * (1.0/sin(phi))))/(2.0 * myUtil::PI);
    if((Vn/Ve)*Vp > 0){
        u = theta;
    }else{
        u = 1 - theta;
    }
    v = 1-v;
    int a = (int)(u*image.width());
    int b = (int)(v*image.height());
    a += image.width()/2.0;
    a = a % image.width();
    if(a != a)
    	a = 0;
    if(b != b)
    	b = 0;
    if(a < 0)
    	a = 0;
    if(a >= image.width())
    	a = image.width()-1;
    if(b < 0)
    	b = 0;
    if(b >= image.height())
    	b = image.height()-1;
    QRgb col = image.pixel(a,b);
	return myUtil::color9D(qRed(col),qGreen(col),qBlue(col),qRed(col),qGreen(col),qBlue(col),qRed(col),qGreen(col),qBlue(col));
}
