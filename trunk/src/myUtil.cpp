/*
 * myUtil.cpp
 *
 *  Created on: 28.10.2011
 *      Author: philipp
 */

#include "myUtil.h"

myUtil::myUtil() {
	// TODO Auto-generated constructor stub

}

myUtil::~myUtil() {
	// TODO Auto-generated destructor stub
}

MyVector myUtil::color(float x, float y, float z){
	MyVector vec(3,0);
	vec(0) = x;
	vec(1) = y;
	vec(2) = z;
	return vec;
}

MyVector myUtil::PosHom(float x, float y, float z){
	MyVector vec(4,1);
	vec(0) = x;
	vec(1) = y;
	vec(2) = z;
	return vec;
}

MyVector myUtil::PosHom(float x, float y, float z, float w){
	MyVector vec(4,0);
	vec(0) = x;
	vec(1) = y;
	vec(2) = z;
	vec(3) = w;
	return vec;
}

MyVector myUtil::Pos5D(float v, float w, float x, float y, float z){
	MyVector vec(5,0);
	vec(0) = v;
	vec(1) = w;
	vec(2) = x;
	vec(3) = y;
	vec(3) = z;
	return vec;
}

MyMatrix myUtil::eye(int dim){
	MyMatrix mat(dim,dim,0);
	for(int i = 0; i < dim; i++){
		mat(i,i) = 1.0;
	}
	return mat;
}

MyVector myUtil::elementWiseMulti(MyVector A, MyVector B){
	int s = A.size();
	if(B.size() < s)
		s = B.size();
	MyVector back(s);
	for(int i = 0; i < s; i++){
		back(i) = A(i)*B(i);
	}
	return back;
}

float myUtil::homogenNorm(MyVector vec){
	if(vec(3) == 0){
		return color(vec(0),vec(1),vec(2)).norm();
	}//else
	MyVector back = vec;
	back *= (1.0/back(3));
	back(3) = 0;
	return back.norm();
}

MyVector myUtil::normalize(MyVector vec){
	if(vec(3) == 0){
		return vec*(1.0f/homogenNorm(vec));
	}
	MyVector back = vec;
	back *= (1.0/back(3));
	back(3) = 1.0;
	back *= (1.0/homogenNorm(back));
	back(3) = 1.0;
	return back;
}
