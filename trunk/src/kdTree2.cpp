/*
 * kdTree2.cpp
 *
 *  Created on: 07.11.2011
 *      Author: philipp
 */

#include "kdTree2.h"
#include <algorithm>
#include <limits>

using namespace std;

bool xsort (SceneObject* i, SceneObject* j) { return (i->getCenter()(0) < j->getCenter()(0)); }
bool ysort (SceneObject* i, SceneObject* j) { return (i->getCenter()(1) < j->getCenter()(1)); }
bool zsort (SceneObject* i, SceneObject* j) { return (i->getCenter()(2) < j->getCenter()(2)); }

kdTree2::kdTree2(std::vector< SceneObject* > objects) {
	//build BoundingBox
	float minX = std::numeric_limits<float>::max();
	float minY = std::numeric_limits<float>::max();
	float minZ = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::min();
	float maxY = std::numeric_limits<float>::min();
	float maxZ = std::numeric_limits<float>::min();

	for(int i = 0; i < objects.size(); i++){
		CVector<float> min = objects.at(i)->getMin();
		CVector<float> max = objects.at(i)->getMax();

		//min
		if(min(0) < minX)
			minX = min(0);
		if(min(1) < minY)
			minY = min(1);
		if(min(2) < minZ)
			minZ = min(2);
		//max
		if(max(0) > maxX)
			maxX = max(0);
		if(max(1) > maxY)
			maxY = max(1);
		if(max(2) > maxZ)
			maxZ = max(2);
	}

	isRoot = true;
	isLeaf = false;
	depth = 0;

	boundingBox = new AABB(minX,maxX,minY,maxY,minZ,maxZ);

	minimum = minX;
	maximum = maxX;

	sort(objects.begin(),objects.end(),xsort);

	std::vector< SceneObject* > vecL;
	std::vector< SceneObject* > vecR;
	int i = 0;
	for(; i < objects.size()/2; i++){
		vecL.push_back(objects[i]);
	}
	for(; i < objects.size(); i++){
		vecR.push_back(objects[i]);
	}

	centerMin = getMax(vecL, depth%3);
	centerMax = getMin(vecR, depth%3);

	left = new kdTree2(vecL, minimum, centerMin, minY, maxY, 1);
	right = new kdTree2(vecR, centerMax, maximum, minY, maxY, 1);
}

kdTree2::kdTree2(std::vector< SceneObject* > objects, float minWidth, float maxWidth, float _minHeight, float _maxHeight, int _depth) {
	isRoot = false;
	minimum = _minHeight;
	maximum = _maxHeight;
	minHeight = minWidth;
	maxHeight = maxWidth;
	depth = _depth;

	if(objects.size() == 1){
		isLeaf = true;
		object = objects[0];
	}else{
		if(depth % 3 == 0)
			sort(objects.begin(),objects.end(),xsort);
		if(depth % 3 == 1)
			sort(objects.begin(),objects.end(),ysort);
		if(depth % 3 == 2)
			sort(objects.begin(),objects.end(),zsort);



		std::vector< SceneObject* > vecL;
		std::vector< SceneObject* > vecR;
		int i = 0;
		for(; i < objects.size()/2; i++){
			vecL.push_back(objects[i]);
		}
		for(; i < objects.size(); i++){
			vecR.push_back(objects[i]);
		}

		float partMin = getMax(vecL, depth%3);
		float partMax = getMin(vecR, depth%3);

		left = new kdTree2(vecL, minimum, partMin, minHeight, maxHeight, depth+1);
		right = new kdTree2(vecR, centerMax, partMax, minHeight, maxHeight, depth+1);
	}
}

kdTree2::~kdTree2() {
	// TODO Auto-generated destructor stub
}

CVector<float> kdTree2::collision(CVector<float> origin, CVector<float> direction, bool* collided, float* t_value, CVector<float>* collisionPoint, CVector<float>* normal, bool isLightRay){
	if(isRoot && !(boundingBox->collision(origin,direction))){
		*collided = false;
		return CVector<float>(3,0);
	}

	bool cuttedLeft;
	float tValueLeft;
	float cuttingValueLeft;
	if(direction((depth+1)%3) == 0){
		cuttingValueLeft = origin((depth+1)%3);
	}else{
		tValueLeft = (centerMin - origin(depth%3))/direction(depth%3);
		cuttingValueLeft = origin((depth+1)%3) + tValueLeft*direction((depth+1)%3);
	}
	if(minHeight < cuttingValueLeft && maxHeight > cuttingValueLeft)
		cuttedLeft = true;

	bool cuttedRight;
	float tValueRight;
	float cuttingValueRight;
	if(direction((depth+1)%3) == 0){
		cuttingValueRight = origin((depth+1)%3);
	}else{
		tValueRight = (centerMax - origin(depth%3))/direction(depth%3);
		cuttingValueRight = origin((depth+1)%3) + tValueRight*direction((depth+1)%3);
	}
	if(minHeight < cuttingValueRight && maxHeight > cuttingValueRight)
		cuttedRight = true;

	if(cuttedLeft || cuttedRight){
		if(tValueLeft < tValueRight){
			float t_val;
			bool col;
			CVector<float> colPnt;
			CVector<float> norm;
			CVector<float> color = left->collision(origin,direction,&col,&t_val,&colPnt,&norm,isLightRay);
			if(col){
				*t_value = t_val;
				*normal = norm;
				*collisionPoint = colPnt;
				return color;
			}else{
				color = right->collision(origin,direction,&col,&t_val,&colPnt,&norm,isLightRay);
				*t_value = t_val;
				*normal = norm;
				*collisionPoint = colPnt;
				return color;
			}
		}else{
			float t_val;
			bool col;
			CVector<float> colPnt;
			CVector<float> norm;
			CVector<float> color = right->collision(origin,direction,&col,&t_val,&colPnt,&norm,isLightRay);
			if(col){
				*t_value = t_val;
				*normal = norm;
				*collisionPoint = colPnt;
				return color;
			}else{
				color = left->collision(origin,direction,&col,&t_val,&colPnt,&norm,isLightRay);
				*t_value = t_val;
				*normal = norm;
				*collisionPoint = colPnt;
				return color;
			}
		}
	}else{
		float t_val;
		bool col;
		CVector<float> colPnt;
		CVector<float> norm;
		if(direction((depth+1)%3) < 0){
			if(cuttingValueRight < minHeight){
				CVector<float> color = left->collision(origin,direction,&col,&t_val,&colPnt,&norm,isLightRay);
				if(col){
					*t_value = t_val;
					*normal = norm;
					*collisionPoint = colPnt;
					return color;
				}
			}else{
				CVector<float> color = right->collision(origin,direction,&col,&t_val,&colPnt,&norm,isLightRay);
				if(col){
					*t_value = t_val;
					*normal = norm;
					*collisionPoint = colPnt;
					return color;
				}
			}
		}else{
			if(cuttingValueRight < minHeight){
				CVector<float> color = right->collision(origin,direction,&col,&t_val,&colPnt,&norm,isLightRay);
				if(col){
					*t_value = t_val;
					*normal = norm;
					*collisionPoint = colPnt;
					return color;
				}
			}else{
				CVector<float> color = left->collision(origin,direction,&col,&t_val,&colPnt,&norm,isLightRay);
				if(col){
					*t_value = t_val;
					*normal = norm;
					*collisionPoint = colPnt;
					return color;
				}
			}
		}
	}
}

float kdTree2::getMax(std::vector< SceneObject* > objects, int depthMod){
	float max = std::numeric_limits<float>::min();
	for(int i = 0; i < objects.size(); i++){
		if(objects[i]->getMax()(depthMod) > max){
			max = objects[i]->getMax()(depthMod);
		}
	}
	return max;
}

float kdTree2::getMin(std::vector< SceneObject* > objects, int depthMod){
	float min = std::numeric_limits<float>::max();
	for(int i = 0; i < objects.size(); i++){
		if(objects[i]->getMin()(depthMod) < min){
			min = objects[i]->getMin()(depthMod);
		}
	}
	return min;
}
