/*
 * Light.cpp
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#include "Light.h"
#include "myUtil.h"

Light::Light() {
	position = CVector<float>(4,0);
	position(3) = 1; //wegen homogen
	direction = CVector<float>(4,1);
	isDirectionalLight = false;
	color = CVector<float>(3,0.5);
}

Light::Light(CVector<float> _position, CVector<float> _direction, bool _directionalLight, CVector<float> _color) {
	position = _position;
	direction = _direction;
	isDirectionalLight = _directionalLight;
	color = _color;
	IDiffuse = color;
	ISpecular = color;//myUtil::color(0.5,0.5,0.5)
}

Light::Light(CVector<float> _position, CVector<float> _direction, bool _directionalLight, CVector<float> _color, CVector<float> _IDiffuse, CVector<float> _ISpecular) {
	position = _position;
	direction = _direction;
	isDirectionalLight = _directionalLight;
	color = _color;
	IDiffuse = _IDiffuse;
	ISpecular = _ISpecular;
}

Light::~Light() {

}
