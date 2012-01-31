/*
 * MyVector.cpp
 *
 *  Created on: 07.11.2011
 *      Author: philipp
 */

#include "MyVector.h"
#include <cmath>

MyVector::MyVector() {
	// TODO Auto-generated constructor stub

}

MyVector::MyVector(int size) {
	data = vector<float>(size);
}

MyVector::MyVector(int size, float fillValue) {
	data = vector<float>(size, fillValue);
}

MyVector::~MyVector() {
	// TODO Auto-generated destructor stub
}

inline float MyVector::norm(){
	float val = 0;
	for(int i = 0; i < data.size(); i++){
		val += data[i]*data[i];
	}
	return sqrt(val);
}

int MyVector::size(){
	return data.size();
}

inline MyVector MyVector::operator- (MyVector other){
	MyVector back(data.size());
	for(int i = 0; i < data.size(); i++){
		back(i) = data[i] - other(i);
	}
	return back;
}

MyVector MyVector::operator*(float val) {
	MyVector back(data.size());
	for (int i = 0; i < data.size(); i++) {
		back(i) = data[i] * val;
	}
	return back;
}

inline MyVector MyVector::operator+ (MyVector other){
	MyVector vec(data.size());
	for(int i = 0; i < data.size(); i++){
		vec(i) = data[i] + other(i);
	}
	return vec;
}

inline float MyVector::operator* (MyVector other){
	float back = 0;
	for(int i = 0; i < data.size(); i++){
		back += data[i] * other(i);
	}
	return back;
}

//inline MyVector& operator* (float val){
//	CVector vec(data.size());
//	for(int i = 0; i < data.size(); i++){
//		vec(i) = data[i]*val;
//	}
//	return vec;
//}

inline MyVector MyVector::operator/ (MyVector other){
	MyVector vec(data.size());
	vec(0) = data[1]*other(2) - data[2]*other(1);
	vec(0) = data[2]*other(0) - data[0]*other(2);
	vec(0) = data[0]*other(1) - data[1]*other(0);
	return vec;
}

void MyVector::operator*= (float val){
	for(int i = 0; i < data.size(); i++){
		data[i] *= val;
	}
}

void MyVector::operator+= (MyVector other){
	for(int i = 0; i < data.size(); i++){
		data[i] += other(i);
	}
}

inline float& MyVector::operator()(int index){
	return data[index];
}
