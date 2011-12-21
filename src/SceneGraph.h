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
#include "myUtil.h"

class SceneGraph {
public:
	SceneGraph(bool pres, int argc, char *argv[]);
	SceneGraph(CMatrix<float> _cameraMatrix, CVector<float> _backgroundColor);
	virtual ~SceneGraph();
	void addLightSource(Light light);
	void addSceneObject(SceneObject* object);
	void setCameraMatrix(CMatrix<float> _cameraMatrix);
	void ProjectionMatrix(CVector<float> _origin, float near, float far, float width, float height);
	CVector<float> castLightRay(CVector<float> origin, CVector<float> direction);
	CVector<float> castRay(CVector<float> origin, CVector<float> direction, int recursionDepth = 2, float reflection = 0.3, float transparency = 0.3);
	CMatrix<float> getCameraMatrix();
	BVH* loadObj(std::string pathToObj, CVector<float> color, CVector<float> origin, CVector<float> angles = CVector<float>(3,0), CVector<float> movement = CVector<float>(3,0));

	CVector<float> getColorForRay(CVector<float> origin, CVector<float> direction, int recurionDepth);
	CVector<float> Recursion(CVector<float> color, CVector<float> originPoint, CVector<float> oldViewingDirection, CVector<float> normal, int recursionDepth, float reflection, float transparency);
	bool castRay(CVector<float> origin, CVector<float> direction, CVector<float>* normal, CVector<float>* collisionPoint, float* refl, float* trans, CVector<float>* color);
	CVector<float> PhongOnPoint(CVector<float> col, CVector<float> pointToEvaluate, CVector<float> normal, CVector<float> direction);
	void loadScene(int scene);
	static CMatrix<float> Rx(float angle);
	static CMatrix<float> Ry(float angle);
	static CMatrix<float> Rz(float angle);
private:
	std::vector<Light> lightSources;
	std::vector<SceneObject*> objects;
	CMatrix<float> cameraMatrix;
	CMatrix<float> inverseCameraMatrix;
	CMatrix<float> origin;
	CVector<float> backgroundColor;
	CVector<float> EAmbient;
	float n;
	CVector<float> Phong(CVector<float> normal, CVector<float> lightdirection, bool seeTheLight, CVector<float> viewingRay, CVector<float> colorVec, CVector<float> intensities, float n);
	bool lightVisible(CVector<float> point, CVector<float> lightDir, float distToLight);
	CMatrix<float> InverseCameraMatrix(CVector<float> cameraPos, CVector<float> lookAt, CVector<float> up);
	bool presentation;
	float t;
};

#endif /* SCENEGRAPH_H_ */
