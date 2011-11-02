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
	static CVector<float> PosHom(float x, float y, float z);
	static CVector<float> PosHom(float x, float y, float z, float w);
	static const float epsi = 0.0001;
	static CMatrix<float> eye(int dim);
	static CVector<float> elementWiseMulti(CVector<float> A, CVector<float> B);
	static float homogenNorm(CVector<float> vec);
	static CVector<float> normalize(CVector<float> vec);
};

#endif /* MYUTIL_H_ */
