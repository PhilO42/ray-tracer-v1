/*
 * Ray.cpp
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#include "Ray.h"

Ray::Ray(MyVector _origin, MyVector _direction) {
	origin = _origin;
	direction = _direction;
	calculatedColor = false;
}

Ray::~Ray() {
	// TODO Auto-generated destructor stub
}

//MyVector Ray::getColor(SceneGraph* graph){
//	if(!calculatedColor){
//		color = graph->getColor();
//	}
//	return color;
//}
