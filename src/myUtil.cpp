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

CVector<float> myUtil::color(float x, float y, float z){
	CVector<float> vec(3,0);
	vec(0) = x;
	vec(1) = y;
	vec(2) = z;
	return vec;
}

CVector<float> myUtil::color9D(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3){
	CVector<float> vec(9,0);
	vec(0) = x1;
	vec(1) = y1;
	vec(2) = z1;
	vec(3) = x2;
	vec(4) = y2;
	vec(5) = z2;
	vec(6) = x3;
	vec(7) = y3;
	vec(8) = z3;
	return vec;
}

CVector<float> myUtil::PosHom(float x, float y, float z){
	CVector<float> vec(4,1);
	vec(0) = x;
	vec(1) = y;
	vec(2) = z;
	return vec;
}

CVector<float> myUtil::PosHom(float x, float y, float z, float w){
	CVector<float> vec(4,0);
	vec(0) = x;
	vec(1) = y;
	vec(2) = z;
	vec(3) = w;
	return vec;
}

CVector<float> myUtil::Pos5D(float v, float w, float x, float y, float z){
	CVector<float> vec(5,0);
	vec(0) = v;
	vec(1) = w;
	vec(2) = x;
	vec(3) = y;
	vec(4) = z;
	return vec;
}

CMatrix<float> myUtil::eye(int dim){
	CMatrix<float> mat(dim,dim,0);
	for(int i = 0; i < dim; i++){
		mat(i,i) = 1.0;
	}
	return mat;
}

CVector<float> myUtil::elementWiseMulti(CVector<float> A, CVector<float> B){
	int s = A.size();
	if(B.size() < s)
		s = B.size();
	CVector<float> back(s);
	for(int i = 0; i < s; i++){
		back(i) = A(i)*B(i);
	}
	return back;
}

float myUtil::homogenNorm(CVector<float> vec){
	if(vec(3) == 0){
		return color(vec(0),vec(1),vec(2)).norm();
	}//else
	CVector<float> back = vec;
	back *= (1.0/back(3));
	back(3) = 0;
	return back.norm();
}

CVector<float> myUtil::normalize(CVector<float> vec){
	if(vec(3) == 0){
		if(homogenNorm(vec) != 0)
		return vec*(1.0f/homogenNorm(vec));
	}
	CVector<float> back = vec;
	back *= (1.0/back(3));
	back(3) = 1.0;
	if(homogenNorm(back) != 0)
		back *= (1.0/homogenNorm(back));
	back(3) = 1.0;
	return back;
}
