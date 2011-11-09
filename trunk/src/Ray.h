/*
 * Ray.h
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#ifndef RAY_H_
#define RAY_H_

#include "mathe/CVector.h"

class Ray {
public:
	Ray(CVector<float> _origin, CVector<float> _direction);
	virtual ~Ray();
	//CVector<float> getColor(SceneGraph* graph);
	CVector<float> origin;
	CVector<float> direction;
private:
	bool calculatedColor;
	CVector<float> color;
};

#endif /* RAY_H_ */
