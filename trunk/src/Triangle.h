/*
 * Triangle.h
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "SceneObject.h"
#include "mathe/CVector.h"
#include <vector>

class Triangle : public SceneObject {
public:
	Triangle();
	virtual ~Triangle();
private:
	std::vector<CVector <float> > corners;
};

#endif /* TRIANGLE_H_ */
