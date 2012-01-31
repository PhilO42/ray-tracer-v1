/*
 * MyMatrix.h
 *
 *  Created on: 07.11.2011
 *      Author: philipp
 */

#ifndef MYMATRIX_H_
#define MYMATRIX_H_

#include <vector>
#include <iostream>
#include "MyVector.h"

using namespace std;

class MyMatrix {
public:
	MyMatrix();
	MyMatrix(int _rows, int _cols);
	MyMatrix(int _rows, int _cols, float fillValue);
	virtual ~MyMatrix();

	void print();
	void inv();
	MyVector operator*(MyVector vec);
	float& operator()(int x, int y);
private:
	int rows;
	int cols;
	float data[4][4];
};

#endif /* MYMATRIX_H_ */
