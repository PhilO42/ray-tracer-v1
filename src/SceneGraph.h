/*
 * SceneGraph.h
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#ifndef SCENEGRAPH_H_
#define SCENEGRAPH_H_

#include "MyMatrix.h"
#include "Light.h"
#include "SceneObject.h"
#include "kdTree.h"

class SceneGraph {
public:
	SceneGraph();
	SceneGraph(MyMatrix _cameraMatrix, MyVector _backgroundColor);
	virtual ~SceneGraph();
	void addLightSource(Light light);
	void addSceneObject(SceneObject* object);
	void setCameraMatrix(MyMatrix _cameraMatrix);
	MyMatrix ProjectionMatrix(MyVector _origin, float near, float far, float width, float height);
	MyVector castRay(MyVector origin, MyVector direction);
	MyMatrix getCameraMatrix();
	MyVector castLightRay(MyVector origin, MyVector direction);
	kdTree* loadObj(std::string pathToObj, MyVector color);
private:
	std::vector<Light> lightSources;
	MyMatrix cameraMatrix;
	MyMatrix inverseCameraMatrix;
	MyMatrix origin;
	std::vector<SceneObject*> objects;
	MyVector backgroundColor;
	MyVector intenseAmbient;
	MyVector intenseDiffuse;
	MyVector intenseSpecular;
	MyVector EAmbient;
	float n;
	MyVector Phong(MyVector normal, MyVector lightdirection, bool seeTheLight, MyVector viewingRay, MyVector EDiffuse, MyVector ESpecular, float n);
	bool lightVisible(MyVector point, MyVector lightDir, float distToLight);
	MyMatrix InverseCameraMatrix(MyVector cameraPos, MyVector lookAt, MyVector up);

	//depricated
	MyMatrix Rx(float angle);
	MyMatrix Ry(float angle);
	MyMatrix Rz(float angle);
	MyVector getColor(MyVector _origin, MyVector direction);
};

#endif /* SCENEGRAPH_H_ */
