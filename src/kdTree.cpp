/*
 * kdTree.cpp
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#include "kdTree.h"
#include <limits>
#include <algorithm>

using namespace std;

bool xsort (SceneObject* i, SceneObject* j) { return (i->getCenter()(0) < j->getCenter()(0)); }
bool ysort (SceneObject* i, SceneObject* j) { return (i->getCenter()(1) < j->getCenter()(1)); }
bool zsort (SceneObject* i, SceneObject* j) { return (i->getCenter()(2) < j->getCenter()(2)); }

kdTree::kdTree(vector< SceneObject* > objects) {
	//build BoundingBox
	float minX = std::numeric_limits<float>::max();
	float minY = std::numeric_limits<float>::max();
	float minZ = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::min();
	float maxY = std::numeric_limits<float>::min();
	float maxZ = std::numeric_limits<float>::min();

	for(int i = 0; i < objects.size(); i++){
		CVector<float> min = objects.at(i)->getMin();
		CVector<float> max = objects.at(i)->getMin();
		//min
		if(min(0) < minX)
			minX = min(0);
		if(min(1) < minY)
			minY = min(1);
		if(min(2) < minZ)
			minZ = min(2);
		//max
		if(max(0) < maxX)
			maxX = max(0);
		if(max(1) < maxY)
			maxY = max(1);
		if(max(2) < maxZ)
			maxZ = max(2);
	}

	isRoot = true;
	boundingBox = AABB(minX,maxX,minY,maxY,minZ,maxZ);

	//build Children
	sort(objects.begin(), objects.end(), xsort);
	depthMod = 0;

	vector< SceneObject* > vecL(0);
	vector< SceneObject* > vecR(0);
	int i = 0;
	for (; i < objects.size() / 2.0; i++) {
		vecL.push_back(objects.at(i));
	}
	for (; i < objects.size(); i++) {
		vecR.push_back(objects.at(i));
	}

	partitionValue = (vecL.at(vecL.size()-1))->getCenter()(0) + (vecR.at(0)->getCenter())(0);

	left = new kdTree(vecL,1);
	right = new kdTree(vecR,1);
}

kdTree::~kdTree() {
	// TODO Auto-generated destructor stub
}

kdTree::kdTree(vector< SceneObject* > objects, int depth){
	if(objects.size() == 1){
		isLeave = true;
		obj = objects.at(0);
	}else{
		if(depth%3 == 0)
			sort(objects.begin(), objects.end(), xsort);
		if(depth%3 == 1)
			sort(objects.begin(), objects.end(), ysort);
		if(depth%3 == 2)
			sort(objects.begin(), objects.end(), zsort);

		depthMod = depth%3;

		vector< SceneObject* > vecL(0);
		vector< SceneObject* > vecR(0);
		int i = 0;
		for(; i < objects.size()/2.0; i++){
			vecL.push_back(objects.at(i));
		}
		for(;i < objects.size(); i++){
			vecR.push_back(objects.at(i));
		}

		partitionValue = ((vecL.at(vecL.size()-1))->getCenter()(depth%3) + (vecR.at(0))->getCenter()(depth%3))/2.0;

		depth++;
		left = new kdTree(vecL,depth);
		right = new kdTree(vecR,depth);
	}
}

CVector<float> kdTree::collision(CVector<float> origin, CVector<float> direction, bool* collided, float* t_value, CVector<float>* collisionPoint, CVector<float>* normal, bool isLightRay){
	if(isLeave)
		return obj->collision(origin,direction,collided,t_value,collisionPoint,normal,isLightRay);

	bool* collidedL;
	float* t_valueL;
	CVector<float>* collisionPointL;
	CVector<float>* normalL;
	bool* collidedR;
	float* t_valueR;
	CVector<float>* collisionPointR;
	CVector<float>* normalR;

	CVector<float> colL = left->collision(origin,direction,collidedL,t_valueL,collisionPointL,normalL,isLightRay);
	CVector<float> colR = right->collision(origin,direction,collidedR,t_valueR,collisionPointR,normalR,isLightRay);
	if(!*collidedL && !*collidedR){
		*collided = false;
		return CVector<float>(3,0);
	}
	if(collidedL && !collidedR){
		*collided = true;
		*t_value = *t_valueL;
		*collisionPoint = *collisionPointL;
		*normal = *normalL;
		return colL;
	}
	if(!collidedL && collidedR){
		*collided = true;
		*t_value = *t_valueR;
		*collisionPoint = *collisionPointR;
		*normal = *normalR;
		return colR;
	}
	if(collidedL && collidedR){
		if(*t_valueL < *t_valueR){
			*collided = true;
			*t_value = *t_valueR;
			*collisionPoint = *collisionPointR;
			*normal = *normalR;
			return colR;
		}
		*collided = true;
		*t_value = *t_valueL;
		*collisionPoint = *collisionPointL;
		*normal = *normalL;
		return colL;
	}
}
