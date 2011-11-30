/*
 * BVH.h
 *
 *  Created on: 08.11.2011
 *      Author: philipp
 */

#ifndef BVH_H_
#define BVH_H_

#include "SceneObject.h"
#include "Triangle.h"
#include "AABB.h"
#include <vector>

using namespace std;

class BVH: public SceneObject {
public:
	BVH(std::vector< Triangle* > objects);
	BVH(std::vector< Triangle* > objects, int _depth);
	CVector<float> collision(CVector<float> origin, CVector<float> direction, bool* collided, float* t_value, CVector<float>* collisionPoint, CVector<float>* normal, bool isLightRay, float* refl, float* trans);
	CVector<float> getCenter();
	virtual ~BVH();
private:
	AABB boundingBox;
	bool isLeaf;
	int depth;
	vector< Triangle* > obj;
	BVH* left;
	BVH* right;
};

#endif /* BVH_H_ */
