/*
 * BVH.cpp
 *
 *  Created on: 08.11.2011
 *      Author: philipp
 */

#include "BVH.h"
#include <limits>
#include <algorithm>

bool xsort (SceneObject* i, SceneObject* j) { return (i->getCenter()(0) < j->getCenter()(0)); }
bool ysort (SceneObject* i, SceneObject* j) { return (i->getCenter()(1) < j->getCenter()(1)); }
bool zsort (SceneObject* i, SceneObject* j) { return (i->getCenter()(2) < j->getCenter()(2)); }

BVH::BVH(std::vector< Triangle* > objects) {
	cout << "number of faces = " << objects.size() << endl;
	float minX = std::numeric_limits<float>::max();
	float minY = std::numeric_limits<float>::max();
	float minZ = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::min();
	float maxY = std::numeric_limits<float>::min();
	float maxZ = std::numeric_limits<float>::min();

	CVector<float> min;
	CVector<float> max;
	int size = objects.size();
	for (int i = 0; i < size; i++) {
		min = objects[i]->getMin();
		max = objects[i]->getMax();
		//min
		if (min(0) < minX)
			minX = min(0);
		if (min(1) < minY)
			minY = min(1);
		if (min(2) < minZ)
			minZ = min(2);
		//max
		if (max(0) > maxX)
			maxX = max(0);
		if (max(1) > maxY)
			maxY = max(1);
		if (max(2) > maxZ)
			maxZ = max(2);
	}

	isLeaf = false;
	boundingBox = AABB(minX, maxX, minY, maxY, minZ, maxZ);
	depth = 0;

	sort(objects.begin(), objects.end(), xsort);

	std::vector<Triangle*> vecL;
	std::vector<Triangle*> vecR;
	int j = 0;
	for (; j < objects.size() / 2; j++) {
		vecL.push_back(objects[j]);
	}
	for (; j < objects.size(); j++) {
		vecR.push_back(objects[j]);
	}

	left = new BVH(vecL, depth+1);
	right = new BVH(vecR, depth+1);

	reflectionValue = 0;
	trancparencyValue = 0;
}

BVH::BVH(std::vector< Triangle* > objects, int _depth) {
	depth = _depth;
	if(objects.size() <= 3){
		isLeaf = true;
		obj = objects;
	}else{
		isLeaf = false;
		float minX = std::numeric_limits<float>::max();
		float minY = std::numeric_limits<float>::max();
		float minZ = std::numeric_limits<float>::max();
		float maxX = std::numeric_limits<float>::min();
		float maxY = std::numeric_limits<float>::min();
		float maxZ = std::numeric_limits<float>::min();

		for (int i = 0; i < objects.size(); i++) {
			CVector<float> min = objects.at(i)->getMin();
			CVector<float> max = objects.at(i)->getMax();

			//min
			if (min(0) < minX)
				minX = min(0);
			if (min(1) < minY)
				minY = min(1);
			if (min(2) < minZ)
				minZ = min(2);
			//max
			if (max(0) > maxX)
				maxX = max(0);
			if (max(1) > maxY)
				maxY = max(1);
			if (max(2) > maxZ)
				maxZ = max(2);
		}
		boundingBox = AABB(minX, maxX, minY, maxY, minZ, maxZ);

		if(depth % 3 == 0)
			sort(objects.begin(),objects.end(),xsort);
		if(depth % 3 == 1)
			sort(objects.begin(),objects.end(),ysort);
		if(depth % 3 == 2)
			sort(objects.begin(),objects.end(),zsort);

		std::vector<Triangle*> vecL;
		std::vector<Triangle*> vecR;
		int i = 0;
		for (; i < objects.size() / 2; i++) {
			vecL.push_back(objects[i]);
		}
		for (; i < objects.size(); i++) {
			vecR.push_back(objects[i]);
		}

		left = new BVH(vecL, depth+1);
		right = new BVH(vecR, depth+1);
	}

	reflectionValue = 0;
	trancparencyValue = 0;
}

BVH::~BVH() {
	if(isLeaf){
		for(int i = obj.size()-1; i >= 0; i--){
			delete obj[i];
		}
	}else{
		delete right;
		delete left;
	}
}

CVector<float> BVH::collision(CVector<float> origin, CVector<float> direction, bool* collided, float* t_value, CVector<float>* collisionPoint, CVector<float>* normal, bool isLightRay, float* refl, float* trans){
	*refl = reflectionValue;
	*trans = trancparencyValue;
	if(isLeaf){
		*collided = false;
		float t_val;
		float t_val_last = numeric_limits<float>::max();
		bool col = false;
		CVector<float> colPnt;
		CVector<float> norm;
		CVector<float> color_best(3,0);
		for(int i = 0; i < obj.size(); i++){
			float a;
			CVector<float> color = obj[i]->collision(origin,direction,&col,&t_val,&colPnt,&norm,isLightRay, &a, &a);
			if(col){
				*collided = true;
				if(t_val < t_val_last){
					t_val_last = t_val;
					*t_value = t_val;
					*refl = obj[i]->reflectionValue;
					*trans = obj[i]->trancparencyValue;
					*collisionPoint = colPnt;
					*normal = norm;
					color_best = color;
				}
			}
		}
		return color_best;
	}
	if(!boundingBox.collision(origin,direction)){
		*collided = false;
		return CVector<float>(3,0);
	}else	{
		float t_valL;
		bool colL = false;
		CVector<float> colPntL;
		CVector<float> normL;
		float reflL;
		float transL;
		CVector<float> colorL = left->collision(origin,direction,&colL,&t_valL,&colPntL,&normL,isLightRay,&reflL,&transL);
		float t_valR;
		bool colR = false;
		CVector<float> colPntR;
		CVector<float> normR;
		float reflR;
		float transR;
		CVector<float> colorR = right->collision(origin,direction,&colR,&t_valR,&colPntR,&normR,isLightRay,&reflR,&transR);
		if(colL && colR){
			*collided = true;
			if(t_valL < t_valR){
				*t_value = t_valL;
				*normal = normL;
				*refl = reflL;
				*trans = transL;
				*collisionPoint = colPntL;
				return colorL;
			}else{
				*t_value = t_valR;
				*normal = normR;
				*refl = reflR;
				*trans = transR;
				*collisionPoint = colPntR;
				return colorR;
			}
		}
		if (colL) {
			*collided = true;
			*t_value = t_valL;
			*normal = normL;
			*collisionPoint = colPntL;
			return colorL;
		}
		if (colR) {
			*collided = true;
			*t_value = t_valR;
			*normal = normR;
			*collisionPoint = colPntR;
			return colorR;
		}
	}
	*collided = false;
	return CVector<float>(3,0);
}

CVector<float> BVH::getCenter(){
	return boundingBox.getCenter();
}
