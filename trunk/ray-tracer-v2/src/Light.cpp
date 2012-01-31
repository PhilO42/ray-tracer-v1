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
	color = CVector<float>(9,0);
}

Light::Light(CVector<float> _position, CVector<float> _direction, bool _directionalLight, CVector<float> _color) {
	position = _position;
	direction = _direction;
	isDirectionalLight = _directionalLight;
	if(_color.size() == 9){
		color = _color;
		intensities = color;
	}else{
		color = myUtil::color9D(_color(0),_color(1),_color(2),_color(0),_color(1),_color(2),_color(0),_color(1),_color(2));
		intensities = myUtil::color9D(color(0),color(1),color(2),color(0),color(1),color(2),color(0),color(1),color(2));
	}
}

Light::Light(CVector<float> _position, CVector<float> _direction, bool _directionalLight, CVector<float> _color, CVector<float> _IAmbient, CVector<float> _IDiffuse, CVector<float> _ISpecular) {
	position = _position;
	direction = _direction;
	isDirectionalLight = _directionalLight;
	if(_color.size() == 9){
		color = _color;
	}else{
		color = myUtil::color9D(_color(0),_color(1),_color(2),_color(0),_color(1),_color(2),_color(0),_color(1),_color(2));
	}
	intensities = myUtil::color9D(_IAmbient(0),_IAmbient(1),_IAmbient(2),_IDiffuse(0),_IDiffuse(1),_IDiffuse(2),_ISpecular(0),_ISpecular(1),_ISpecular(2));
}

Light::~Light() {

}
