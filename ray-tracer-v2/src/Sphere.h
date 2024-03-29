/*
 * Sphere.h
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include "SceneObject.h"
#include "mathe/CVector.h"
#include <QImage>

class Sphere : public SceneObject{
public:
	Sphere();
	Sphere(float _radius, CVector<float> _position, CVector<float> _color, float refl=0.3, float trans=0.3);
	Sphere(float _radius, CVector<float> _position, CVector<float> _color, float refl, float trans, std::string path);
	Sphere(float _radius, CVector<float> _position, CVector<float> _color, bool _isLight);
	virtual ~Sphere();
	CVector<float> collision(CVector<float> origin, CVector<float> direction, bool* collided, float* t_value, CVector<float>* collisionpoint, CVector<float>* normal, bool isLightRay, float* refl, float* trans);
	CVector<float> getMin();
	CVector<float> getMax();
	CVector<float> getCenter();
	void rotate(CVector<float> angles);
	CVector<float> getColor(CVector<float> collisionPoint);
private:
	float radius;
	CVector<float> position;
	CVector<float> color;
	bool isLight;
	CVector<float> min;
	CVector<float> max;
	QImage image;
	bool textured;
};

#endif /* SPHERE_H_ */
