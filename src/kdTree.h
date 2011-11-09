/*
 * kdTree.h
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#ifndef KDTREE_H_
#define KDTREE_H_

#include "SceneObject.h"
#include "mathe/CVector.h"
#include <vector>
#include "AABB.h"

class kdTree : public SceneObject{
public:
	kdTree(std::vector< SceneObject* > objects);
	kdTree(std::vector< SceneObject* > objects, float _minVal, float _maxVal, int depth);
	virtual ~kdTree();
	CVector<float> collision(CVector<float> origin, CVector<float> direction, bool* collided, float* t_value, CVector<float>* collisionPoint, CVector<float>* normal, bool isLightRay);
	float minVal;
	float maxVal;
private:
	kdTree* left;
	kdTree* right;
	bool isLeave;
	bool isRoot;
	AABB boundingBox;//only if root
	int depthMod;
	float partitionValue;
	SceneObject* obj;
	float getMinVal(std::vector< SceneObject* > objects, int dim);
	float getMaxVal(std::vector< SceneObject* > objects, int dim);
};

#endif /* KDTREE_H_ */
