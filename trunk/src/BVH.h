/*
 * BVH.h
 *
 *  Created on: 08.11.2011
 *      Author: philipp
 */

#ifndef BVH_H_
#define BVH_H_

#include "SceneObject.h"
#include "AABB.h"
#include <vector>

using namespace std;

class BVH: public SceneObject {
public:
	BVH(std::vector< SceneObject* > objects);
	BVH(std::vector< SceneObject* > objects, int _depth);
	CVector<float> collision(CVector<float> origin, CVector<float> direction, bool* collided, float* t_value, CVector<float>* collisionPoint, CVector<float>* normal, bool isLightRay);
	virtual ~BVH();
private:
	AABB boundingBox;
	bool isLeaf;
	int depth;
	vector< SceneObject* > obj;
	BVH* left;
	BVH* right;
};

#endif /* BVH_H_ */
