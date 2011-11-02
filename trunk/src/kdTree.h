/*
 * kdTree.h
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#ifndef KDTREE_H_
#define KDTREE_H_

#include "SceneObject.h"
#include "Triangle.h"
#include <vector>

class kdTree : public SceneObject{
public:
	kdTree();
	virtual ~kdTree();
private:
	std::vector<Triangle> triangles;//TODO should be exchange by a tree
};

#endif /* KDTREE_H_ */
