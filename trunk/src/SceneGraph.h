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
#include "BVH.h"

class SceneGraph {
public:
	SceneGraph();
	SceneGraph(CMatrix<float> _cameraMatrix, CVector<float> _backgroundColor);
	virtual ~SceneGraph();
	void addLightSource(Light light);
	void addSceneObject(SceneObject* object);
	void setCameraMatrix(CMatrix<float> _cameraMatrix);
	CMatrix<float> ProjectionMatrix(CVector<float> _origin, float near, float far, float width, float height);
	CVector<float> castLightRay(CVector<float> origin, CVector<float> direction);
	CVector<float> castRay(CVector<float> origin, CVector<float> direction, int recursionDepth = 2, float reflection = 0.3, float transparency = 0.3);
	CMatrix<float> getCameraMatrix();
	BVH* loadObj(std::string pathToObj, CVector<float> color, CVector<float> origin);

	CVector<float> getColorForRay(CVector<float> color, CVector<float> origin, CVector<float> direction, int recurionDepth, float reflection, float transparency);
	CVector<float> Recursion(CVector<float> color, CVector<float> originPoint, CVector<float> oldViewingDirection, CVector<float> normal, int recursionDepth, float reflection, float transparency);
	bool castRay(CVector<float> origin, CVector<float> direction, CVector<float>* normal, CVector<float>* collisionPoint, float* refl, float* trans, CVector<float>* color);
	CVector<float> PhongOnPoint(CVector<float> col, CVector<float> pointToEvaluate, CVector<float> normal, CVector<float> direction);
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
