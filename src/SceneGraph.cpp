/*
 * SceneGraph.cpp
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#include "SceneGraph.h"
#include <iostream>
#include <cmath>
#include <limits>
#include <fstream>
#include <iostream>
#include <QString>
#include <QObject>
#include <QStringList>
#include "myUtil.h"
#include "mathe/CVector.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Box.h"
#include "Cylinder.h"

using namespace std;

SceneGraph::SceneGraph(bool pres){
	presentation = pres;
	lightSources = std::vector<Light>(0);
	objects = std::vector<SceneObject*>(0);
	inverseCameraMatrix = InverseCameraMatrix(myUtil::PosHom(4.2,2,0), myUtil::PosHom(0,0,0), myUtil::PosHom(0,1,0));
	//setCameraMatrix(CVector<float>(3,0),1,10,4,6);
	backgroundColor = myUtil::color(127.0f,219.0f,255.0f);
	EAmbient = myUtil::color(0.6,0.6,0.6);

//	addLightSource(Light(myUtil::PosHom(2,4,-3), CVector<float>(4,1), false, myUtil::color(0.5,0.5,0)));
//	addLightSource(Light(myUtil::PosHom(-2,4,-3), CVector<float>(4,1), false, myUtil::color(0.5,0.5,0)));
//	addLightSource(Light(myUtil::PosHom(6,0,0), CVector<float>(4,1), false, myUtil::color(0.5,0.5,0)));
//	addLightSource(Light(myUtil::PosHom(-1,3,0), CVector<float>(4,1), false, myUtil::color(0.5,0.5,0.9)));
//	addLightSource(Light(myUtil::PosHom(0,1,0), myUtil::PosHom(0,1,1,0), true, myUtil::color(0.8,0.8,0.8)));

	intenseAmbient = myUtil::color(100,100,100);
	intenseDiffuse = myUtil::color(100,100,100);
	intenseSpecular = myUtil::color(250,250,250);
	n = 5;

//	CVector<float> pos = myUtil::PosHomogen(0,6,-1);
//	pos = inverseCameraMatrix * pos;
//	cout << pos << endl;
//	CVector<float> col = myUtil::Pos(255,0,0);
//	Sphere* s = new Sphere(2.0,pos,col);
//	objects.push_back(s);
//	CVector<float> pos1 = myUtil::PosHomogen(2,4,0);
//	pos1 = inverseCameraMatrix * pos1;
//	CVector<float> col1 = myUtil::Pos(255,255,0);
//	Sphere* s1 = new Sphere(1.1,pos1,col1);
//	objects.push_back(s1);
//	CVector<float> pos2 = myUtil::PosHomogen(-1,7,-2);
//	pos2 = inverseCameraMatrix * pos2;
//	CVector<float> col2 = myUtil::Pos(0,0,255);
//	Sphere* s2 = new Sphere(2.5,pos2,col2);
//	objects.push_back(s2);
//	{CVector<float> pos = myUtil::PosHom(1,2,3);
//		 CVector<float> col = myUtil::color(0,0,0);
//		 Sphere* s = new Sphere(0.1, pos, col);
////	objects.push_back(s);
//	}
//	//koordinatensystem
//	Triangle* tr = new Triangle(myUtil::PosHom(1,0,0), myUtil::PosHom(0,1,0), myUtil::PosHom(0,0,1), myUtil::PosHom(1,1,1), myUtil::color(255,255,0));
//	objects.push_back(tr);
//		Plane* p = new Plane(myUtil::PosHom(0,0,0),myUtil::PosHom(1,0,0), myUtil::PosHom(0,0,1), myUtil::PosHom(0,1,0),myUtil::color(255,0,255));
//		objects.push_back(p);

	//test szene
}

SceneGraph::SceneGraph(CMatrix<float> _cameraMatrix, CVector<float> _backgroundColor){
	lightSources = std::vector<Light>(0);
	objects = std::vector<SceneObject*>(0);
	cameraMatrix = _cameraMatrix;
	backgroundColor = _backgroundColor;
}

SceneGraph::~SceneGraph() {
	for(int i = objects.size()-1; i >= 0; i--){
		delete objects[i];
	}
}

CMatrix<float> SceneGraph::InverseCameraMatrix(CVector<float> cameraPos, CVector<float> lookAt, CVector<float> up){
//	//nach: http://www.opengl.org/sdk/docs/man/xhtml/gluLookAt.xml
//	CMatrix<float> mat(4,4,0);
//	CVector<float> direction = lookAt - cameraPos;
//	direction *= (1.0/direction.norm());
//	up *= (1.0/up.norm());
//	CVector<float> side = direction/up;//cross product
//	side *= (1.0/side.norm());
//	CVector<float> u = side/direction;
//	mat(0,0) = side(0);
//	mat(0,1) = side(1);
//	mat(0,2) = side(2);
//	mat(1,0) = u(0);
//	mat(1,1) = u(1);
//	mat(1,2) = u(2);
//	mat(2,0) = -direction(0);
//	mat(2,1) = -direction(1);
//	mat(2,2) = -direction(2);
////	mat(0,3) = cameraPos(0);
////	mat(1,3) = cameraPos(1);
////	mat(2,3) = cameraPos(2);
//	mat(3,3) = 1.0;
//	mat.inv();
//	cout << mat << endl;

	//this function calculates a new basis
	CMatrix<float> mat(4,4,0);

	//calculate view-direction
	CVector<float> view = lookAt - cameraPos;
	view = myUtil::normalize(view);

	//calculate vector to side
	CVector<float> side = view / up;
	side = myUtil::PosHom(side(0),side(1),side(2),0);
	side = myUtil::normalize(side);

	//calculate new up-vector
	//so no re-orthogonalization is needed
	up = side / view;
	up = myUtil::PosHom(up(0),up(1),up(2),0);
	up = myUtil::normalize(up);

	//this three vectors building an new orthigonal basis
	mat(0,0) = side(0);
	mat(1,0) = side(1);
	mat(2,0) = side(2);
	mat(0,1) = up(0);
	mat(1,1) = up(1);
	mat(2,1) = up(2);
	mat(0,2) = -view(0);
	mat(1,2) = -view(1);
	mat(2,2) = -view(2);

	//the homogen 1 is needed
	mat(3,3) = 1.0;

	//rotate the cameraposition
	cameraPos = mat * myUtil::PosHom(cameraPos(0), cameraPos(1), cameraPos(2));

	//the inverse transformation is neede so invert the matrix
	//mat.inv();

	//put in the inverse translation
	mat(3,0) = -cameraPos(0);
	mat(3,1) = -cameraPos(1);
	mat(3,2) = -cameraPos(2);

	cout << mat << endl;

	return mat;
}

void SceneGraph::addLightSource(Light light){
	CVector<float> pos = light.position;
	CVector<float> col = myUtil::color(100000,100000,100000);
	if(!light.isDirectionalLight){
		Sphere* s = new Sphere(0.1,pos,col,true);
		objects.push_back(s);
	}
	lightSources.push_back(light);
}

void SceneGraph::addSceneObject(SceneObject* object){
	objects.push_back(object);
}

void SceneGraph::setCameraMatrix(CMatrix<float> _cameraMatrix){
	cameraMatrix = _cameraMatrix;
}

CMatrix<float> SceneGraph::ProjectionMatrix(CVector<float> _origin, float near, float far, float width, float height){
//	origin = _origin;

	cameraMatrix = CMatrix<float>(4,4,0);
	float right  = -width/2.0;
	float left   =  width/2.0;
	float top    =  height/2.0;
	float bottom = -height/2.0;

	cameraMatrix(0,0) = near/right;
	cameraMatrix(1,1) = near/top;
	cameraMatrix(2,2) = (far+near)/(far-near);
	cameraMatrix(3,2) = 1.0;
	cameraMatrix(2,3) = -(2*far*near)/(far-near);
//	mat(1,2) = -(top+bottom)/(top-bottom);
//	mat(0,2) = -(right+left)/(right-left);
}


CVector<float> SceneGraph::getColor(CVector<float> _origin, CVector<float> direction){
	CVector<float> color = backgroundColor;
	//TODO get collisions und ersetzte Farbe wenn collision
}

CVector<float> SceneGraph::castRay(CVector<float> origin, CVector<float> direction, int recursionDepth, float reflection, float transparency){
	CVector<float> bestColor = this->backgroundColor;
	CVector<float> normal;
	CVector<float> collisionPoint;
	CVector<float> bestNormal;
	CVector<float> bestCollisionPoint;
	float refl;
	float trans;
	float bestRefl;
	float bestTrans;
	float t = std::numeric_limits<float>::max();
	bool hit = false;

	for(int i = 0; i < objects.size(); i++){
		bool collided = false;
		float distance = 0;
		CVector<float> color = objects[i]->collision(origin, direction, &collided, &distance, &collisionPoint, &normal, false, &refl, &trans);
		if(collided){
			if(distance < t){
				hit = true;
				t = distance;
				bestRefl = refl;
				bestTrans = trans;
				bestColor = color;
				bestNormal = normal;
				bestCollisionPoint = collisionPoint;
			}
		}
	}
	if(hit){
		if(recursionDepth != 1){
			bestColor = Recursion(bestColor, origin, direction, bestNormal, recursionDepth, bestRefl, bestTrans);
		}else{
			CVector<float> col(3,0);
			for(int i = 0; i < lightSources.size(); i++){
				CVector<float> EDiffuse = lightSources[i].IDiffuse;
				CVector<float> ESpecular = lightSources[i].ISpecular;
				CVector<float> lightDir;

				if(lightSources.at(i).isDirectionalLight){
					//directional light
					lightDir = lightSources[i].direction;
	//				lightDir *= -1.0;
					lightDir = myUtil::normalize(lightDir);
					col += Phong(bestNormal,lightDir,lightVisible(bestCollisionPoint, lightSources[i].direction, numeric_limits<float>::max()), direction, EDiffuse, ESpecular, n);
				}else{
					//point light
					lightDir = lightSources[i].position - bestCollisionPoint;

					//abnahme der helligkeit mit der distanz
					float dist = myUtil::homogenNorm(lightDir);
					if(dist != 0)
						EDiffuse *= (1.0/dist*dist);
					if(dist != 0)
						ESpecular *= (1.0/dist*dist);
					lightDir = myUtil::normalize(lightDir);
					col += Phong(bestNormal,lightDir,lightVisible(bestCollisionPoint, lightDir, dist), direction, EDiffuse, ESpecular, n);
				}
			}
			col *= (1.0f/lightSources.size());
			bestColor = myUtil::elementWiseMulti(bestColor,EAmbient);
			bestColor += col;
	//		cout << bestColor << endl;
		}
	}
	return bestColor;
}

bool SceneGraph::lightVisible(CVector<float> point, CVector<float> lightDir, float distToLight){
//	if(directionalLight)
//		return true;
	CVector<float> bestColor = this->backgroundColor;
	CVector<float> normal;
	CVector<float> collisionPoint;
//	CVector<float> bestNormal;
//	CVector<float> bestCollisionPoint;
	float t = distToLight;
//	bool hit = false;
	for(int i = 0; i < objects.size(); i++){
		bool collided = false;
		float distance = myUtil::epsi;
		CVector<float> color;
		float a;
		color = objects[i]->collision(point, myUtil::normalize(lightDir), &collided, &distance, &collisionPoint, &normal, true, &a, &a);
		if(collided){
			if(distance < t){
				return false;
//				hit = true;
//				t = distance;
//				bestColor = color;
//				bestNormal = normal;
//				bestCollisionPoint = collisionPoint;
			}
		}
	}
	return true;
}

CVector<float> SceneGraph::Phong(CVector<float> normal, CVector<float> lightdirection, bool seeTheLight, CVector<float> viewingRay, CVector<float> EDiffuse, CVector<float> ESpecular, float n){
	normal = myUtil::normalize(normal);
//	if(normal(0) != normal(0)){//nan
//		return CVector<float>(3,0);
//	}
	lightdirection = myUtil::normalize(lightdirection);
	viewingRay = myUtil::normalize(viewingRay);
	CVector<float> R = 2.0f * (lightdirection * normal) * normal - lightdirection;
	if(myUtil::homogenNorm(R) > 0)
		R = myUtil::normalize(R);
	CVector<float> color(3,0);

	if(seeTheLight){
		color += myUtil::elementWiseMulti(intenseDiffuse, EDiffuse) * abs(normal * lightdirection);
		color += myUtil::elementWiseMulti(intenseSpecular, ESpecular) * (float)pow(abs(R * viewingRay), 10);
	}
	return color;
}

CMatrix<float> SceneGraph::Rx(float angle){
	CMatrix<float> mat(4,4,0);
	mat(0,0) = 1.0;
	mat(1,1) = cos(angle);
	mat(1,2) = -sin(angle);
	mat(2,1) = sin(angle);
	mat(2,2) = cos(angle);
	mat(3,3) = 1.0;

	return mat;
}

CMatrix<float> SceneGraph::Ry(float angle){
	CMatrix<float> mat(4,4,0);
	mat(0,0) = cos(angle);
	mat(0,2) = sin(angle);
	mat(2,0) = -sin(angle);
	mat(2,2) = cos(angle);
	mat(1,1) = 1.0;
	mat(3,3) = 1.0;

	return mat;
}

CMatrix<float> SceneGraph::Rz(float angle){
	CMatrix<float> mat(4,4,0);
	mat(0,0) = cos(angle);
	mat(0,1) = -sin(angle);
	mat(2,2) = 1.0;
	mat(1,0) = sin(angle);
	mat(1,1) = cos(angle);
	mat(3,3) = 1.0;

	return mat;
}

CMatrix<float> SceneGraph::getCameraMatrix(){
	CMatrix<float> mat = inverseCameraMatrix;
	mat.inv();
	return mat;
}

CVector<float> SceneGraph::castLightRay(CVector<float> origin, CVector<float> direction){
	CVector<float> bestColor = this->backgroundColor;
	CVector<float> normal;
	CVector<float> collisionPoint;
	CVector<float> bestNormal;
	CVector<float> bestCollisionPoint;
	float t = numeric_limits<float>::max();
	bool hit = false;

	for(int i = 0; i < objects.size(); i++){
		bool collided = false;
		float distance = 0;
		float a;
		CVector<float> color = objects[i]->collision(origin, direction, &collided, &distance, &collisionPoint, &normal, false, &a, &a);
		if(collided){
			if(distance < t){
				hit = true;
				t = distance;
				bestColor = color;
				bestNormal = normal;
				bestCollisionPoint = collisionPoint;
			}
		}
	}
	if(hit){
		CVector<float> col(3,0);
		for(int i = 0; i < lightSources.size(); i++){
			CVector<float> lightDir = lightSources[i].position - bestCollisionPoint;
			if(lightSources[i].isDirectionalLight){
				CVector<float> dir = lightSources[i].direction;
				if(lightVisible(bestCollisionPoint,dir,numeric_limits<float>::max())){
					col += myUtil::color(255,255,255);
				}
			}else{
				CVector<float> dir = lightSources[i].position - bestCollisionPoint;
				if(lightVisible(bestCollisionPoint,myUtil::normalize(dir), myUtil::homogenNorm(dir))){
					col += myUtil::color(255,255,255);
				}
			}
		}
		col *= (1.0f/lightSources.size());
		bestColor = col;
	}
	return bestColor;
}

BVH* SceneGraph::loadObj(string pathToObj, CVector<float> color, CVector<float> origin){
	//file load
	std::vector<QString> text;
	ifstream file;
	file.open(pathToObj.c_str());
	if (file) {
		while (!file.eof()) {
			string buffer;
			getline(file, buffer);
			text.push_back(QObject::tr(buffer.c_str()));
		}
		file.close();
	} else {
		cout << "cant open file \"" << pathToObj << "\"";
	}

	vector< CVector<float> > vertices;
	vertices.push_back(CVector<float>(3,0));//damit ab 1 gezählt werden kann
	vector< CVector<float> > normals;
	normals.push_back(CVector<float>(3,0));//damit ab 1 gezählt werden kann
	vector< SceneObject* > triangles;

	for(int i = 0; i < text.size(); i++){
		if(text.at(i).size() < 2)
			continue;//too short

		//vertex
		if(text.at(i)[0] == 'v' && !(text.at(i)[1] == 't') && !(text.at(i)[1] == 'n')){
			QStringList list = text.at(i).split(" ");
			float nums[3];
			int ind = 0;
			for(int j = 1; j < list.size() && ind < 3; j++){
				if(list.at(j).toStdString() == " " || list.at(j).toStdString() == "  " || list.at(j).size() == 0)
					continue;
				nums[ind] = list.at(j).toFloat();
				ind++;
			}
			vertices.push_back(myUtil::color(nums[0],nums[1],nums[2])+origin);
		}

		//texture coordinate
		if(text.at(i)[0] == 'v' && text.at(i)[1] == 't'){
			//TODO if needed
		}

		//normal
		if(text.at(i)[0] == 'v' && text.at(i)[1] == 'n'){
			QStringList list = text.at(i).split(" ");
			float nums[3];
			int ind = 0;
			for(int j = 1; j < list.size() && ind < 3; j++){
				if(list.at(j).toStdString() == " " || list.at(j).toStdString() == "  " || list.at(j).size() == 0)
					continue;
				nums[ind] = list.at(j).toFloat();
				ind++;
			}
			normals.push_back(myUtil::PosHom(nums[0],nums[1],nums[2]));
		}

		//triangle
		if(text.at(i)[0] == 'f'){
			QStringList list = text.at(i).split(" ");
			float verts[3];
			float norms[3];
			int ind = 0;
			for(int j = 1; j < list.size() && ind < 3; j++){
				if(list.at(j).toStdString() == " " || list.at(j).toStdString() == "  " || list.at(j).size() == 0)
					continue;
				QStringList list2 = list.at(j).split("/");
				verts[ind] = list2[0].toInt();
				norms[ind] = list2[2].toInt();
				++ind;
			}
//			objects.push_back(new Triangle(vertices.at(verts[0]), vertices.at(verts[1]), vertices.at(verts[2]), normals.at(norms[0]), normals.at(norms[1]), normals.at(norms[2]), color));
			triangles.push_back(new Triangle(vertices.at(verts[0]), vertices.at(verts[1]), vertices.at(verts[2]), normals.at(norms[0]), normals.at(norms[1]), normals.at(norms[2]), color));
		}
	}

	BVH* bvh = new BVH(triangles);
	objects.push_back(bvh);
	return bvh;
}

CVector<float> SceneGraph::Recursion(CVector<float> color, CVector<float> originPoint, CVector<float> oldViewingDirection, CVector<float> normal, int recursionDepth, float reflection, float transparency){
	if(recursionDepth == 0)
		return PhongOnPoint(color, originPoint, normal, oldViewingDirection);
	CVector<float> invView = oldViewingDirection;
	invView *= -1.0;
	CVector<float> R = myUtil::normalize((2.0f * (normal * invView) * normal) - invView);
	CVector<float> normT;
	CVector<float> colPntT;
	float reflT;
	float transT;
	CVector<float> normR;
	CVector<float> colPntR;
	CVector<float> colorR;
	CVector<float> colorT;
	float reflR;
	float transR;
	bool hitT = castRay(originPoint, oldViewingDirection, &normT, &colPntT, &reflT, &transT, &colorT);
	bool hitR = castRay(originPoint, R, &normR, &colPntR, &reflR, &transR, &colorR);
	CVector<float> colT = CVector<float>(3,0);
	CVector<float> colR = CVector<float>(3,0);
	if(!hitR){
		colR = backgroundColor;
	}else{
		if(reflection != 0)
			colR = Recursion(colorR, colPntR, R, normR, recursionDepth-1, reflR, transR);
	}
	if (!hitT) {
		colT = backgroundColor;
	} else {
		if(transparency != 0)
			colT = Recursion(colorT, colPntT, oldViewingDirection, normT, recursionDepth - 1, reflT, transT);
	}
	return (1-transparency-reflection) * PhongOnPoint(color, originPoint, normal, oldViewingDirection) + reflection * colR + transparency * colT;
}

CVector<float> SceneGraph::getColorForRay(CVector<float> color, CVector<float> origin, CVector<float> direction, int recurionDepth, float reflection, float transparency){
	CVector<float> norm;
	CVector<float> colPnt;
	CVector<float> col;
	float refl;
	float trans;
	bool hit = castRay(origin, direction, &norm, &colPnt, &refl, &trans, &col);
	if(!hit){
		return backgroundColor;
	}else{
		return Recursion(col, colPnt, direction, norm, recurionDepth-1, refl, trans);
	}
}

bool SceneGraph::castRay(CVector<float> origin, CVector<float> direction, CVector<float>* normal, CVector<float>* collisionPoint, float* refl, float* trans, CVector<float>* color){
	CVector<float> norm;
	CVector<float> colPoint;
	float reflect;
	float transp;
	float bestReflect;
	float bestTransp;
	CVector<float> col;
	CVector<float> bestCol;
	CVector<float> bestNormal;
	CVector<float> bestCollisionPoint;
	float t = std::numeric_limits<float>::max();
	bool hit = false;

	for(int i = 0; i < objects.size(); i++){
		bool collided = false;
		float distance = 0;
		col = objects[i]->collision(origin, direction, &collided, &distance, &colPoint, &norm, false, &reflect, &transp);
		if(collided){
			if(distance < t){
				hit = true;
				t = distance;
				bestNormal = norm;
				bestCol = col;
				bestReflect = reflect;
				bestTransp = transp;
				bestCollisionPoint = colPoint;
			}
		}
	}
	if(hit){
		*color = bestCol;
		*trans = bestTransp;
		*refl = bestReflect;
		*collisionPoint = bestCollisionPoint;
		*normal = bestNormal;
		return true;
	}
	return false;
}

CVector<float> SceneGraph::PhongOnPoint(CVector<float> col, CVector<float> pointToEvaluate, CVector<float> normal, CVector<float> direction){
	for(int i = 0; i < lightSources.size(); i++){
		CVector<float> EDiffuse = lightSources[i].IDiffuse;
		CVector<float> ESpecular = lightSources[i].ISpecular;
		CVector<float> lightDir;

		if(lightSources.at(i).isDirectionalLight){
			//directional light
			lightDir = lightSources[i].direction;
			lightDir = myUtil::normalize(lightDir);
			col += Phong(normal,lightDir,lightVisible(pointToEvaluate, lightSources[i].direction, numeric_limits<float>::max()), direction, EDiffuse, ESpecular, n);
		}else{
			//point light
			lightDir = lightSources[i].position - pointToEvaluate;

			//abnahme der helligkeit mit der distanz
			float dist = myUtil::homogenNorm(lightDir);
			if(dist != 0)
				EDiffuse *= (1.0/dist*dist);
			if(dist != 0)
				ESpecular *= (1.0/dist*dist);
			lightDir = myUtil::normalize(lightDir);
			col += Phong(normal,lightDir,lightVisible(pointToEvaluate, lightDir, dist), direction, EDiffuse, ESpecular, n);
		}
	}
	return col;
}

void SceneGraph::loadScene(int scene){
	lightSources = std::vector<Light>(0);
	objects = std::vector<SceneObject*>(0);
	switch(scene){
		case 0:
			//4 balls
			inverseCameraMatrix = InverseCameraMatrix(myUtil::PosHom(4.2,2,0), myUtil::PosHom(0,0,0), myUtil::PosHom(0,1,0));
			objects.push_back(new Plane(myUtil::PosHom(4,-4.25,-7), myUtil::PosHom(-4,-4.25,-7), myUtil::PosHom(4,-4.25,7),myUtil::PosHom(0,1,0),myUtil::PosHom(0,0,0),0,0,"white.png"));
			objects.push_back(new Sphere(0.5, myUtil::PosHom(0.7,0.75,0.9), myUtil::color(255, 255, 0), 0, 0));
			objects.push_back(new Sphere(0.5, myUtil::PosHom(0,0.75,1.6), myUtil::color(0, 255, 30),0,0));
			objects.push_back(new Sphere(0.5, myUtil::PosHom(0,0.75,0.5), myUtil::color(255, 0, 0),0,0));
			objects.push_back(new Sphere(1.5, myUtil::PosHom(-2,1.75,-0.5), myUtil::color(0, 0, 255),0,0));
			addLightSource(Light(myUtil::PosHom(-2,4,-1), CVector<float>(4,1), false, myUtil::color(0.5,0.5,0.5)));
			addLightSource(Light(myUtil::PosHom(0,1,0), myUtil::PosHom(0,1,0,0), true, myUtil::color(0.5,0.5,0.5)));
			break;
		case 1:
			//axis
			inverseCameraMatrix = InverseCameraMatrix(myUtil::PosHom(4,4,4), myUtil::PosHom(0,0,0), myUtil::PosHom(0,1,0));
				addLightSource(Light(myUtil::PosHom(2,0,2), CVector<float>(4,1), false, myUtil::color(0.5,0.5,0.5)));
				addLightSource(Light(myUtil::PosHom(2,4,2), CVector<float>(4,1), false, myUtil::color(0.5,0.5,0.5)));
			for(int i = 1; i < 4; i++){
				objects.push_back(new Sphere(0.4,myUtil::PosHom(i,0,0),myUtil::color(255,0,0)));
				objects.push_back(new Sphere(0.4,myUtil::PosHom(0,i,0),myUtil::color(0,255,0)));
				objects.push_back(new Sphere(0.4,myUtil::PosHom(0,0,i),myUtil::color(0,0,255)));
			
				objects.push_back(new Sphere(0.4,myUtil::PosHom(i,i,0),myUtil::color(255,255,0)));
				objects.push_back(new Sphere(0.4,myUtil::PosHom(0,i,i),myUtil::color(0,255,255)));
				objects.push_back(new Sphere(0.4,myUtil::PosHom(i,0,i),myUtil::color(255,0,255)));
			}
			break;
		case 2:
			//cool scene
			inverseCameraMatrix = InverseCameraMatrix(myUtil::PosHom(4.2,2,0), myUtil::PosHom(0,0,0), myUtil::PosHom(0,1,0));
			objects.push_back(new Sphere(0.5, myUtil::PosHom(0.7,0.75,0.9), myUtil::color(255, 255, 0), 0, 0.8));
			objects.push_back(new Sphere(0.5, myUtil::PosHom(0,0.75,1.7), myUtil::color(0, 255, 30),0,0));
			objects.push_back(new Sphere(0.5, myUtil::PosHom(0,0.75,0.5), myUtil::color(0, 0, 0),0.8,0));
			loadObj("models/lamp.obj", myUtil::color(255,0,0), myUtil::PosHom(-0.5,2.7,1.5));
			addLightSource(Light(myUtil::PosHom(-0.5,2.7,1.5), CVector<float>(4,1), false, myUtil::color(0.9,0.9,0.9)));
			objects.push_back(new Sphere(0.4, myUtil::PosHom(-0.5,2.7,1.5), myUtil::color(10000000, 10000000, 10000000), true));
			objects.push_back(new Plane(myUtil::PosHom(8,-4.25,-14), myUtil::PosHom(-8,-4.25,-14), myUtil::PosHom(8,-4.25,14),myUtil::PosHom(0,1,0),myUtil::PosHom(0,0,0),0,0.3,"tile.jpg","tile_bump.jpg"));
			objects.push_back(new Box(myUtil::PosHom(0,0,0), myUtil::PosHom(4.5,0.5,6.5), myUtil::color(0,255,255),0,0,"wood.jpg"));
			objects.push_back(new Cylinder(myUtil::PosHom(1.8,-4.25,2.8), 4, myUtil::color(255,0,0),0.3));
			objects.push_back(new Cylinder(myUtil::PosHom(-1.8,-4.25,2.8), 4, myUtil::color(255,0,0),0.3));
			objects.push_back(new Cylinder(myUtil::PosHom(1.8,-4.25,-2.8), 4, myUtil::color(255,0,0),0.3));
			objects.push_back(new Cylinder(myUtil::PosHom(-1.8,-4.25,-2.8), 4, myUtil::color(255,0,0),0.3));
			addLightSource(Light(myUtil::PosHom(-0.5,-1,1.5), CVector<float>(4,1), false, myUtil::color(0.9,0.9,0.9)));
		break;
		case 3:
			//cool scene + duck
			inverseCameraMatrix = InverseCameraMatrix(myUtil::PosHom(4.2,2,0), myUtil::PosHom(0,0,0), myUtil::PosHom(0,1,0));
			objects.push_back(new Sphere(0.5, myUtil::PosHom(0.7,0.75,0.9), myUtil::color(255, 255, 0), 0, 0.8));
			objects.push_back(new Sphere(0.5, myUtil::PosHom(0,0.75,1.7), myUtil::color(0, 255, 30),0,0));
			objects.push_back(new Sphere(0.5, myUtil::PosHom(0,0.75,0.5), myUtil::color(0, 0, 0),0.8,0));
			loadObj("models/lamp.obj", myUtil::color(255,0,0), myUtil::PosHom(-0.5,2.7,1.5));
			addLightSource(Light(myUtil::PosHom(-0.5,2.7,1.5), CVector<float>(4,1), false, myUtil::color(0.9,0.9,0.9)));
			objects.push_back(new Sphere(0.4, myUtil::PosHom(-0.5,2.7,1.5), myUtil::color(10000000, 10000000, 10000000), true));
			objects.push_back(new Plane(myUtil::PosHom(8,-4.25,-14), myUtil::PosHom(-8,-4.25,-14), myUtil::PosHom(8,-4.25,14),myUtil::PosHom(0,1,0),myUtil::PosHom(0,0,0),0,0,"tile.jpg","tile_bump.jpg"));
			objects.push_back(new Box(myUtil::PosHom(0,0,0), myUtil::PosHom(4.5,0.5,6.5), myUtil::color(0,255,255),0,0,"wood.jpg"));
			objects.push_back(new Cylinder(myUtil::PosHom(1.8,-4.25,2.8), 4, myUtil::color(255,0,0),0.3));
			objects.push_back(new Cylinder(myUtil::PosHom(-1.8,-4.25,2.8), 4, myUtil::color(255,0,0),0.3));
			objects.push_back(new Cylinder(myUtil::PosHom(1.8,-4.25,-2.8), 4, myUtil::color(255,0,0),0.3));
			objects.push_back(new Cylinder(myUtil::PosHom(-1.8,-4.25,-2.8), 4, myUtil::color(255,0,0),0.3));
			addLightSource(Light(myUtil::PosHom(-0.5,-1,1.5), CVector<float>(4,1), false, myUtil::color(0.9,0.9,0.9)));
			loadObj("models/ducky2.obj", myUtil::color(255,255,0), myUtil::PosHom(0.5,0.25,-1));
			objects.push_back(new Sphere(0.06,myUtil::PosHom(0.998662,2.10338,-0.498035), myUtil::color(0,0,0)));
			objects.push_back(new Sphere(0.06,myUtil::PosHom(0.002,2.10338,-0.498035), myUtil::color(0,0,0)));
		break;
	}
}
