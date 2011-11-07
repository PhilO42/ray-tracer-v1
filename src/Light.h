/*
 * Light.h
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#ifndef LIGHT_H_
#define LIGHT_H_

#include "MyMatrix.h"

class Light {
public:
	Light();
	Light(MyVector _position, MyVector _direction, bool _directionalLight, MyVector _color);
	Light(MyVector _position, MyVector _direction, bool _directionalLight, MyVector _color, MyVector _IDiffuse, MyVector _ISpecular);
	virtual ~Light();
	MyVector position;
	MyVector direction;
	MyVector IDiffuse;
	MyVector ISpecular;
	bool isDirectionalLight;
private:
	MyVector color;
};

#endif /* LIGHT_H_ */
