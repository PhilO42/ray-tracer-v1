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

	leftVal = minX;
	rightVal = maxX;
	upVal = maxY;
	downVal = minY;

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

	left = new kdTree2(vecL, leftVal, getMax(vecL, depth%3), downVal, upVal, 1);
	right = new kdTree2(vecR, getMin(vecR, depth%3), rightVal, downVal, upVal, 1);
}

kdTree2::kdTree2(std::vector< SceneObject* > objects, float _leftVal, float _rightVal, float _downVal, float _upVal, int _depth) {
	isRoot = false;
	leftVal = _leftVal;
	rightVal = _rightVal;
	downVal = _downVal;
	upVal = _upVal;
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

		left = new kdTree2(vecL, leftVal, getMax(vecL, depth%3), downVal, upVal, depth+1);
		right = new kdTree2(vecR, getMin(vecR, depth%3), rightVal, downVal, upVal, depth+1);
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
	if(isLeaf){
		float t_val;
		bool col = false;
		CVector<float> colPnt;
		CVector<float> norm;
		CVector<float> color = object->collision(origin,direction,&col,&t_val,&colPnt,&norm,isLightRay);
		*collided = col;
		if(col)
			cout << "++" << endl;
		*t_value = t_val;
		*normal = norm;
		*collisionPoint = colPnt;
		return color;
	}else{
		float t_valL;
		bool colL = false;
		CVector<float> colPntL;
		CVector<float> normL;
		CVector<float> colorL = left->collision(origin,direction,&colL,&t_valL,&colPntL,&normL,isLightRay);
		float t_valR;
		bool colR = false;
		CVector<float> colPntR;
		CVector<float> normR;
		CVector<float> colorR = right->collision(origin,direction,&colR,&t_valR,&colPntR,&normR,isLightRay);
		if(colR && colL){
			*collided = true;
			if(t_valL < t_valR){
				*t_value = t_valL;
				*normal = normL;
				*collisionPoint = colPntL;
				return colorL;
			}else{
				*t_value = t_valR;
				*normal = normR;
				*collisionPoint = colPntR;
				return colorR;
			}
		}
		if(colL){
			*collided = true;
			*t_value = t_valL;
			*normal = normL;
			*collisionPoint = colPntL;
			return colorL;
		}
		if(colR){
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
//CVector<float> kdTree2::collision(CVector<float> origin, CVector<float> direction, bool* collided, float* t_value, CVector<float>* collisionPoint, CVector<float>* normal, bool isLightRay){
//	if(isRoot && !(boundingBox->collision(origin,direction))){
//		*collided = false;
//		return CVector<float>(3,0);
//	}
//	if(isLeaf){
//		float t_val;
//		bool col = false;
//		CVector<float> colPnt;
//		CVector<float> norm;
//		CVector<float> color = object->collision(origin,direction,&col,&t_val,&colPnt,&norm,isLightRay);
//		*collided = col;
//		if(col)
//			cout << "++" << endl;
//		*t_value = t_val;
//		*normal = norm;
//		*collisionPoint = colPnt;
//		return color;
//	}else{
//		bool cuttedLeft = false;
//		float tValueLeft = std::numeric_limits<float>::max();
//		float cuttingValueLeft;
//		if(direction(depth%3) != 0){
//			tValueLeft = (left->rightVal - origin(depth%3))/direction(depth%3);
//			cuttingValueLeft = origin((depth+1)%3) + tValueLeft*direction((depth+1)%3);
//			if(downVal < cuttingValueLeft && upVal > cuttingValueLeft)
//				cuttedLeft = true;
//		}
//
//		bool cuttedRight = false;
//		float tValueRight = std::numeric_limits<float>::max();
//		float cuttingValueRight;
//		if(direction(depth%3) != 0){
//			tValueRight = (right->leftVal - origin(depth%3))/direction(depth%3);
//			cuttingValueRight = origin((depth+1)%3) + tValueRight*direction((depth+1)%3);
//			if(downVal < cuttingValueRight && upVal > cuttingValueRight)
//				cuttedRight = true;
//		}
//
//	//	if(cuttedLeft || cuttedRight){
//			if(tValueLeft < tValueRight){
//				float t_val = 0;
//				bool col = false;
//				CVector<float> colPnt(4);
//				CVector<float> norm(4);
//				CVector<float> color = left->collision(origin,direction,&col,&t_val,&colPnt,&norm,isLightRay);
//				*collided = col;
//				if(col){
//					*t_value = t_val;
//					*normal = norm;
//					*collisionPoint = colPnt;
//					return color;
//				}else{
//					color = right->collision(origin,direction,&col,&t_val,&colPnt,&norm,isLightRay);
//					*collided = col;
//					*t_value = t_val;
//					*normal = norm;
//					*collisionPoint = colPnt;
//					return color;
//				}
//			}else{
//				float t_val = 0;
//				bool col = false;
//				CVector<float> colPnt(4);
//				CVector<float> norm(4);
//				CVector<float> color = right->collision(origin,direction,&col,&t_val,&colPnt,&norm,isLightRay);
//				if(col){
//					*t_value = t_val;
//					*normal = norm;
//					*collisionPoint = colPnt;
//					return color;
//				}else{
//					color = left->collision(origin,direction,&col,&t_val,&colPnt,&norm,isLightRay);
//					*t_value = t_val;
//					*normal = norm;
//					*collisionPoint = colPnt;
//					return color;
//				}
//			}
//	//	}else{
//	//		float t_val;
//	//		bool col;
//	//		CVector<float> colPnt;
//	//		CVector<float> norm;
//	//		if(direction((depth+1)%3) < 0){
//	//			if(cuttingValueRight < downVal){
//	//				CVector<float> color = left->collision(origin,direction,&col,&t_val,&colPnt,&norm,isLightRay);
//	//				if(col){
//	//					*t_value = t_val;
//	//					*normal = norm;
//	//					*collisionPoint = colPnt;
//	//					return color;
//	//				}
//	//			}else{
//	//				CVector<float> color = right->collision(origin,direction,&col,&t_val,&colPnt,&norm,isLightRay);
//	//				if(col){
//	//					*t_value = t_val;
//	//					*normal = norm;
//	//					*collisionPoint = colPnt;
//	//					return color;
//	//				}
//	//			}
//	//		}else{
//	//			if(cuttingValueRight < downVal){
//	//				CVector<float> color = right->collision(origin,direction,&col,&t_val,&colPnt,&norm,isLightRay);
//	//				if(col){
//	//					*t_value = t_val;
//	//					*normal = norm;
//	//					*collisionPoint = colPnt;
//	//					return color;
//	//				}
//	//			}else{
//	//				CVector<float> color = left->collision(origin,direction,&col,&t_val,&colPnt,&norm,isLightRay);
//	//				if(col){
//	//					*t_value = t_val;
//	//					*normal = norm;
//	//					*collisionPoint = colPnt;
//	//					return color;
//	//				}
//	//			}
//	//		}
//	//	}
//	}
//	return CVector<float>(3,0);
//}

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
