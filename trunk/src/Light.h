/*
 * Light.h
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#ifndef LIGHT_H_
#define LIGHT_H_

#include "mathe/CMatrix.h"

class Light {
public:
	Light();
	Light(CVector<float> _position, CVector<float> _direction, bool _directionalLight, CVector<float> _color);
	Light(CVector<float> _position, CVector<float> _direction, bool _directionalLight, CVector<float> _color, CVector<float> _IDiffuse, CVector<float> _ISpecular);
	virtual ~Light();
	CVector<float> position;
	CVector<float> direction;
	CVector<float> IDiffuse;
	CVector<float> ISpecular;
	bool isDirectionalLight;
private:
	CVector<float> color;
};

#endif /* LIGHT_H_ */
