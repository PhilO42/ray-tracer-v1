/*
 * myUtil.h
 *
 *  Created on: 28.10.2011
 *      Author: philipp
 */

#ifndef MYUTIL_H_
#define MYUTIL_H_

#include "MyVector.h"
#include "MyMatrix.h"
#include <iostream>

using namespace std;

class myUtil {
public:
	myUtil();
	virtual ~myUtil();
	static MyVector color(float x, float y, float z);
	static MyVector PosHom(float x, float y, float z);
	static MyVector PosHom(float x, float y, float z, float w);
	static MyVector Pos5D(float v, float w, float x, float y, float z);
	static const float epsi = 0.0001;
	static MyMatrix eye(int dim);
	static MyVector elementWiseMulti(MyVector A, MyVector B);
	static float homogenNorm(MyVector vec);
	static MyVector normalize(MyVector vec);
};

#endif /* MYUTIL_H_ */
