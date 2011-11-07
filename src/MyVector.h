/*
 * MyVector.h
 *
 *  Created on: 07.11.2011
 *      Author: philipp
 */

#ifndef MYVECTOR_H_
#define MYVECTOR_H_

#include <vector>

using namespace std;

class MyVector {
public:
	MyVector();
	MyVector(int size);
	MyVector(int size, float fillValue);
	virtual ~MyVector();

	float norm();
	int size();
	MyVector operator- (MyVector other);
	MyVector operator+ (MyVector other);
	float operator* (MyVector other);
	MyVector operator* (float val);
	MyVector operator/ (MyVector other);
	void operator*= (float val);
	void operator+= (MyVector other);
	float& operator()(int index);
private:
	vector<float> data;
};

//MyVector operator* (const float val, const MyVector vec);
//MyVector operator* (const MyVector vec, const float val);

#endif /* MYVECTOR_H_ */
