/*
 * AABB.h
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#ifndef AABB_H_
#define AABB_H_

#include "SceneObject.h"
#include "mathe/CVector.h"

class AABB : public SceneObject{
public:
	AABB();
	virtual ~AABB();
private:
	CVector<float> size;
	CVector<float> position;
};

#endif /* AABB_H_ */
