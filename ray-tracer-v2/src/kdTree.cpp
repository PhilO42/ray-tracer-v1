/*
 * kdTree.cpp
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#include "kdTree.h"
#include <limits>
#include <algorithm>
#include "myUtil.h"

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
//		cout << "min " << min << endl;
		CVector<float> max = objects.at(i)->getMax();
//		cout << "max " << max << endl;

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
	isLeave = false;
	boundingBox = AABB(minX,maxX,minY,maxY,minZ,maxZ);
	minVal = minX;
	maxVal = maxX;

	//build Childrens
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

	partitionValue = ((vecL.at(vecL.size()-1))->getCenter()(0) + (vecR.at(0)->getCenter())(0))/2.0;

	cout << "range: " << depthMod << " : " << minVal << " " << partitionValue << " " << maxVal << endl;
	left = new kdTree(vecL,minVal,getMaxVal(vecL,0),1);
	right = new kdTree(vecR,getMinVal(vecR,0),maxVal,1);
}

kdTree::kdTree(vector< SceneObject* > objects, float _minVal, float _maxVal, int depth){
	isRoot = false;
	minVal = _minVal;
	maxVal = _minVal;
	if(objects.size() == 1){
		isLeave = true;
		obj = objects.at(0);
	}else{
		isLeave = false;
		if(depth%3 == 0)
			sort(objects.begin(), objects.end(), xsort);
		if(depth%3 == 1)
			sort(objects.begin(), objects.end(), ysort);
		if(depth%3 == 2)
			sort(objects.begin(), objects.end(), zsort);

		depthMod = depth%3;

		vector< SceneObject* > vecL;
		vector< SceneObject* > vecR;
		int i = 0;
		for(; i < objects.size()/2.0; i++){
			vecL.push_back(objects.at(i));
		}
		for(;i < objects.size(); i++){
			vecR.push_back(objects.at(i));
		}

		partitionValue = ((vecL.at(vecL.size()-1))->getCenter()(depth%3) + (vecR.at(0))->getCenter()(depth%3))/2.0;

		if(!(minVal < partitionValue) || !(maxVal > partitionValue)){
			cout << "range: " << depthMod << " : " << minVal << " " << partitionValue << " " << maxVal << endl;
			cout << (vecL.at(vecL.size()-1))->getCenter()(depth%3)  << " -- " << (vecR.at(0))->getCenter()(depth%3) << endl;
			cout << (vecR.at(vecR.size()-1))->getCenter()(depth%3)  << " -- " << (vecL.at(0))->getCenter()(depth%3) << " s: " << vecR.size() << " " << vecL.size() << endl;
			for(int i = 0; i < vecL.size(); i++){
				cout << vecL[i]->getMin() << " " << vecL[i]->getCenter() << " " << vecL[i]->getMax() << endl;
			}
			for(int i = 0; i < vecR.size(); i++){
				cout << vecR[i]->getMin() << " " << vecR[i]->getCenter() << " " << vecR[i]->getMax() << endl;
			}
		}
		left = new kdTree(vecL, minVal, getMaxVal(vecL,depth), depth+1);
		right = new kdTree(vecR, getMinVal(vecR,depth), maxVal, depth+1);
	}
}

kdTree::~kdTree() {
	// TODO Auto-generated destructor stub
}

float kdTree::getMinVal(vector< SceneObject* > objects, int dim){//0=x, 1= y, 2=z
	float minimum = std::numeric_limits<float>::max();

	for(int i = 0; i < objects.size(); i++){
		CVector<float> min = objects.at(i)->getMin();
		//min
		if(min(dim) < minimum)
			minimum = min(dim);
	}
	return minimum;
}

//get left or right (up and down or front and back) bound (not the center)
float kdTree::getMaxVal(vector< SceneObject* > objects, int dim){
	float maximum = std::numeric_limits<float>::min();

	for(int i = 0; i < objects.size(); i++){
		CVector<float> max = objects.at(i)->getMax();
		//max
		if(max(dim) > maximum)
			maximum = max(dim);
	}
	return maximum;
}

CVector<float> kdTree::collision(CVector<float> origin, CVector<float> direction, bool* collided, float* t_value, CVector<float>* collisionPoint, CVector<float>* normal, bool isLightRay){
	if(isRoot && !boundingBox.collision(origin,direction)){
		*collided = false;
		return CVector<float>(3,0);
	}
	if(isLeave){
		float t_val;
		bool col;
		CVector<float> colPnt;
		CVector<float> norm;
		CVector<float> color = obj->collision(origin,direction,&col,&t_val,&colPnt,&norm,isLightRay);
		*collided = col;
		if(col){
			cout << "bla" << endl;
			*t_value = t_val;
			*collisionPoint = colPnt;
			*normal = norm;
			return color;
		}
		return CVector<float>(3,0);
	}

	//left intersection Value
	float cuttingValueLeft;
	float tLeft;
	bool IntersectionHappendLeft;
	if(direction((depthMod + 1) % 3) == 0) {//value is zero into the desired direction
		cuttingValueLeft = origin(depthMod);
	} else {
		float tLeft = (left->maxVal - origin((depthMod + 1) % 3)) / direction((depthMod + 1) % 3);
		cuttingValueLeft = origin(depthMod) + tLeft * direction(depthMod);
	}
	if(left->minVal <= cuttingValueLeft && left->maxVal >= cuttingValueLeft)
		IntersectionHappendLeft = true;

	//right intersection Value
	float cuttingValueRight;
	float tRight;
	bool IntersectionHappendRight;
	if (direction((depthMod + 1) % 3) == 0) {//value is zero into the desired direction
		cuttingValueRight = origin(depthMod);
	} else {
		float tRight = (right->minVal - origin((depthMod + 1) % 3)) / direction((depthMod + 1) % 3);
		cuttingValueRight = origin(depthMod) + tRight * direction(depthMod);
	}
	if(right->minVal <= cuttingValueRight && right->maxVal >= cuttingValueRight)
		IntersectionHappendRight = true;

	//front to back manner
	bool collisionLeft;
	bool collisionRight;
	float t;
	CVector<float> colPoint;
	CVector<float> norm;
	CVector<float> colo;
	if(IntersectionHappendLeft || IntersectionHappendRight){
		if(tLeft < tRight){
			t = cuttingValueLeft;
			colo = left->collision(origin,direction,&collisionLeft,&t,&colPoint,&norm,isLightRay);
			if(!collisionLeft){
				t = cuttingValueRight;
				colo = right->collision(origin,direction,&collisionLeft,&t,&colPoint,&norm,isLightRay);
			}
			*collided = collisionLeft;
			*t_value = t;
			*collisionPoint = colPoint;
			*normal = norm;
			return colo;
		}else{
			t = cuttingValueRight;
			colo = right->collision(origin,direction,&collisionRight,&t,&colPoint,&norm,isLightRay);
			if(!collisionRight){
				t = cuttingValueLeft;
				colo = left->collision(origin,direction,&collisionRight,&t,&colPoint,&norm,isLightRay);
			}
			*collided = collisionRight;
			*t_value = t;
			*collisionPoint = colPoint;
			*normal = norm;
			return colo;
		}
	}else{
		if(*t_value < right->minVal){
			//check right side
			t = cuttingValueRight;
			colo = right->collision(origin,direction,&collisionRight,&t,&colPoint,&norm,isLightRay);
			*collided = collisionRight;
			*t_value = t;
			*collisionPoint = colPoint;
			*normal = norm;
			return colo;
		}
		if(*t_value > left->maxVal){//&t_value = oldvalue entrypoint
			//check left side
			t = cuttingValueLeft;
			colo = left->collision(origin,direction,&collisionLeft,&t,&colPoint,&norm,isLightRay);
			*collided = collisionLeft;
			*t_value = t;
			*collisionPoint = colPoint;
			*normal = norm;
			return colo;
		}
	}
//	else{//not both //truncating one or both sides
//		if(IntersectionHappendLeft){
//			colo = left->collision(origin,direction,&collisionLeft,&t,&colPoint,&norm,isLightRay);
//			*collided = collisionLeft;
//			*t_value = t;
//			*collisionPoint = colPoint;
//			*normal = norm;
//			return colo;
//		}
//		if(IntersectionHappendRight){
//			colo = right->collision(origin,direction,&collisionRight,&t,&colPoint,&norm,isLightRay);
//			*collided = collisionRight;
//			*t_value = t;
//			*collisionPoint = colPoint;
//			*normal = norm;
//			return colo;
//		}
	*collided = false;
	return myUtil::color(255,0,0);
}

//CVector<float> kdTree::collision(CVector<float> origin, CVector<float> direction, bool* collided, float* t_value, CVector<float>* collisionPoint, CVector<float>* normal, bool isLightRay){
//	if(isRoot && !boundingBox.collision(origin,direction)){
//		*collided = false;
////		cout << "¬1" << endl;
//		return CVector<float>(3,0);
//	}
//	if(isLeave){
////		cout << "leav" << endl;
//		float t_val;
//		bool col;
//		CVector<float> colPnt;
//		CVector<float> norm;
//		CVector<float> color =  obj->collision(origin,direction,collided,&t_val,&colPnt,&norm,isLightRay);
//		*collided = col;
//		if(col){
//			*t_value = t_val;
//			*collisionPoint = colPnt;
//			*normal = norm;
//			return color;
//		}
//		return CVector<float>(3,0);
//	}
//
//	bool collidedL;
//	float t_valueL;
//	CVector<float> collisionPointL;
//	CVector<float> normalL;
//	bool collidedR;
//	float t_valueR;
//	CVector<float> collisionPointR;
//	CVector<float> normalR;
//
//	CVector<float> colL = left->collision(origin,direction,&collidedL,&t_valueL,&collisionPointL,&normalL,isLightRay);
//	CVector<float> colR = right->collision(origin,direction,&collidedR,&t_valueR,&collisionPointR,&normalR,isLightRay);
//	if(!collidedL && !collidedR){
//		*collided = false;
////		cout << "¬2" << endl;
//		return CVector<float>(3,0);
//	}
//	if(collidedL && !collidedR){
//		*collided = true;
//		*t_value = t_valueL;
//		*collisionPoint = collisionPointL;
//		*normal = normalL;
////		cout << "1" << endl;
//		return colL;
//	}
//	if(!collidedL && collidedR){
//		*collided = true;
//		*t_value = t_valueR;
//		*collisionPoint = collisionPointR;
//		*normal = normalR;
////		cout << "2" << endl;
//		return colR;
//	}
//	if(collidedL && collidedR){
//		if(t_valueL < t_valueR){
//			*collided = true;
//			*t_value = t_valueR;
//			*collisionPoint = collisionPointR;
//			*normal = normalR;
////			cout << "3a" << endl;
//			return colR;
//		}
//		*collided = true;
//		*t_value = t_valueL;
//		*collisionPoint = collisionPointL;
//		*normal = normalL;
////		cout << "3b" << endl;
//		return colL;
//	}
//	return myUtil::color(255,0,0);
//}
