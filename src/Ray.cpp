/*
 * Ray.cpp
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#include "Ray.h"

Ray::Ray(CVector<float> _origin, CVector<float> _direction) {
	origin = _origin;
	direction = _direction;
	calculatedColor = false;
}

Ray::~Ray() {
	// TODO Auto-generated destructor stub
}

//CVector<float> Ray::getColor(SceneGraph* graph){
//	if(!calculatedColor){
//		color = graph->getColor();
//	}
//	return color;
//}
