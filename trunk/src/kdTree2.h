/*
 * kdTree2.h
 *
 *  Created on: 07.11.2011
 *      Author: philipp
 */

#ifndef KDTREE2_H_
#define KDTREE2_H_

#include "SceneObject.h"
#include "mathe/CVector.h"
#include <vector>
#include "AABB.h"

class kdTree2 : public SceneObject {
public:
	kdTree2(std::vector< SceneObject* > objects);
	kdTree2(std::vector< SceneObject* > objects, float minWidth, float maxWidth, float _minHeight, float _maxHeight, int _depth);
	virtual ~kdTree2();
	CVector<float> collision(CVector<float> origin, CVector<float> direction, bool* collided, float* t_value, CVector<float>* collisionPoint, CVector<float>* normal, bool isLightRay);
private:
	AABB* boundingBox;
	SceneObject* object;
	int depth;
	float minimum;
	float centerMin;//centerMax <= centerMin
	float centerMax;
	float maximum;
	float maxHeight;
	float minHeight;
	kdTree2* left;
	kdTree2* right;
	bool isLeaf;
	bool isRoot;
	float getMin(std::vector< SceneObject* > objects, int depthMod);
	float getMax(std::vector< SceneObject* > objects, int depthMod);
};

#endif /* KDTREE2_H_ */
