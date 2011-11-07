/*
 * Light.cpp
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#include "Light.h"
#include "myUtil.h"

Light::Light() {
	position = MyVector(4,0);
	position(3) = 1; //wegen homogen
	direction = MyVector(4,1);
	isDirectionalLight = false;
	color = MyVector(3,0.5);
}

Light::Light(MyVector _position, MyVector _direction, bool _directionalLight, MyVector _color) {
	position = _position;
	direction = _direction;
	isDirectionalLight = _directionalLight;
	color = _color;
	IDiffuse = color;
	ISpecular = color;//myUtil::color(0.5,0.5,0.5)
}

Light::Light(MyVector _position, MyVector _direction, bool _directionalLight, MyVector _color, MyVector _IDiffuse, MyVector _ISpecular) {
	position = _position;
	direction = _direction;
	isDirectionalLight = _directionalLight;
	color = _color;
	IDiffuse = _IDiffuse;
	ISpecular = _ISpecular;
}

Light::~Light() {

}
