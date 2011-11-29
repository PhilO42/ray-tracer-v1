/*
 * myUtil.h
 *
 *  Created on: 28.10.2011
 *      Author: philipp
 */

#ifndef MYUTIL_H_
#define MYUTIL_H_

#include "mathe/CVector.h"
#include "mathe/CMatrix.h"
#include <iostream>

using namespace std;

class myUtil {
public:
	myUtil();
	virtual ~myUtil();
	static CVector<float> color(float x, float y, float z);
	static CVector<float> color9D(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
	static CVector<float> PosHom(float x, float y, float z);
	static CVector<float> PosHom(float x, float y, float z, float w);
	static CVector<float> Pos5D(float v, float w, float x, float y, float z);
	static const float epsi = 0.0001;
	static CMatrix<float> eye(int dim);
	static CVector<float> elementWiseMulti(CVector<float> A, CVector<float> B);
	static float homogenNorm(CVector<float> vec);
	static CVector<float> normalize(CVector<float> vec);
};

#endif /* MYUTIL_H_ */
