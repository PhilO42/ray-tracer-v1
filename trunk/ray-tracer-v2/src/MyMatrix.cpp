/*
 * MyMatrix.cpp
 *
 *  Created on: 07.11.2011
 *      Author: philipp
 */

#include "MyMatrix.h"

MyMatrix::MyMatrix() {
	// TODO Auto-generated constructor stub
}

MyMatrix::MyMatrix(int _rows, int _cols) {
	if(rows != 4 || cols != 4){
		cerr << "help MyMatrix size != 4x4" << endl;
	}
	rows = _rows;
	cols = _cols;
}

MyMatrix::MyMatrix(int _rows, int _cols, float fillValue) {
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			data[i][j] = fillValue;
		}
	}
}

MyMatrix::~MyMatrix() {
	// TODO Auto-generated destructor stub
}

void MyMatrix::print(){
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			cout << data[j][i] << " ";
		}
		cout << endl;
	}
}

void MyMatrix::inv(){
	cerr << "you must implement MyMatrix.inv() first bevore usage" << endl;
}

inline MyVector MyMatrix::operator* (MyVector vec){
	MyVector back(4,0);
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			back(i) += data[j][i]*vec(i);
		}
	}
}

inline float& MyMatrix::operator()(int x, int y){
	return data[x][y];
}
