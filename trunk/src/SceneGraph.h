/*
 * SceneGraph.h
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#ifndef SCENEGRAPH_H_
#define SCENEGRAPH_H_

#include "mathe/CMatrix.h"
#include "Light.h"
#include "SceneObject.h"
#include "kdTree.h"

class SceneGraph {
public:
	SceneGraph();
	SceneGraph(CMatrix<float> _cameraMatrix, CVector<float> _backgroundColor);
	virtual ~SceneGraph();
	void addLightSource(Light light);
	void addSceneObject(SceneObject* object);
	void setCameraMatrix(CMatrix<float> _cameraMatrix);
	CMatrix<float> ProjectionMatrix(CVector<float> _origin, float near, float far, float width, float height);
	CVector<float> castRay(CVector<float> origin, CVector<float> direction);
	CMatrix<float> getCameraMatrix();
	CVector<float> castLightRay(CVector<float> origin, CVector<float> direction);
	kdTree* loadObj(std::string pathToObj, CVector<float> color);
private:
	std::vector<Light> lightSources;
	CMatrix<float> cameraMatrix;
	CMatrix<float> inverseCameraMatrix;
	CMatrix<float> origin;
	std::vector<SceneObject*> objects;
	CVector<float> backgroundColor;
	CVector<float> intenseAmbient;
	CVector<float> intenseDiffuse;
	CVector<float> intenseSpecular;
	CVector<float> EAmbient;
	float n;
	CVector<float> Phong(CVector<float> normal, CVector<float> lightdirection, bool seeTheLight, CVector<float> viewingRay, CVector<float> EDiffuse, CVector<float> ESpecular, float n);
	bool lightVisible(CVector<float> point, CVector<float> lightDir, float distToLight);
	CMatrix<float> InverseCameraMatrix(CVector<float> cameraPos, CVector<float> lookAt, CVector<float> up);

	//depricated
	CMatrix<float> Rx(float angle);
	CMatrix<float> Ry(float angle);
	CMatrix<float> Rz(float angle);
	CVector<float> getColor(CVector<float> _origin, CVector<float> direction);
};

#endif /* SCENEGRAPH_H_ */
