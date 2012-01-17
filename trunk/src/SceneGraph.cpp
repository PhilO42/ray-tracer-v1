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

SceneGraph::SceneGraph(bool pres, int argc, char *argv[]){
	t = 0.0;
	if(argc > 1){
		t = atof(argv[1]);
	}
	presentation = pres;
	lightSources = std::vector<Light>(0);
	objects = std::vector<SceneObject*>(0);
	inverseCameraMatrix = InverseCameraMatrix(myUtil::PosHom(4.2,2,0), myUtil::PosHom(0,0,0), myUtil::PosHom(0,1,0));
	//setCameraMatrix(CVector<float>(3,0),1,10,4,6);
	backgroundColor = myUtil::color9D(127.0f,219.0f,255.0f,127.0f,219.0f,255.0f,127.0f,219.0f,255.0f);

//	addLightSource(Light(myUtil::PosHom(2,4,-3), CVector<float>(4,1), false, myUtil::color(0.5,0.5,0)));
//	addLightSource(Light(myUtil::PosHom(-2,4,-3), CVector<float>(4,1), false, myUtil::color(0.5,0.5,0)));
//	addLightSource(Light(myUtil::PosHom(6,0,0), CVector<float>(4,1), false, myUtil::color(0.5,0.5,0)));
//	addLightSource(Light(myUtil::PosHom(-1,3,0), CVector<float>(4,1), false, myUtil::color(0.5,0.5,0.9)));
//	addLightSource(Light(myUtil::PosHom(0,1,0), myUtil::PosHom(0,1,1,0), true, myUtil::color(0.8,0.8,0.8)));
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

	return mat;
}

void SceneGraph::addLightSource(Light light){
	CVector<float> pos = light.position;
	CVector<float> col = myUtil::color9D(100000,100000,100000,100000,100000,100000,100000,100000,100000);
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

void SceneGraph::ProjectionMatrix(CVector<float> _origin, float near, float far, float width, float height){
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

CVector<float> SceneGraph::castRay(CVector<float> origin, CVector<float> direction, int recursionDepth, float reflection, float transparency){
	CVector<float> bestColorVec = myUtil::color(this->backgroundColor(0),this->backgroundColor(1),this->backgroundColor(2));
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
		CVector<float> colorVec = objects[i]->collision(origin, direction, &collided, &distance, &collisionPoint, &normal, false, &refl, &trans);
		if(collided){
			if(distance < t){
				hit = true;
				t = distance;
				bestRefl = refl;
				bestTrans = trans;
				bestColorVec = colorVec;
				bestNormal = normal;
				bestCollisionPoint = collisionPoint;
			}
		}
	}
	if(hit){
		if(recursionDepth != 1){
			bestColorVec = Recursion(bestColorVec, origin, direction, bestNormal, recursionDepth, bestRefl, bestTrans);
		}else{
			CVector<float> col(3,0);
			for(int i = 0; i < lightSources.size(); i++){
				CVector<float> intensities = lightSources[i].intensities;
				CVector<float> lightDir;

				if(lightSources.at(i).isDirectionalLight){
					//directional light
					lightDir = lightSources[i].direction;
	//				lightDir *= -1.0;
					lightDir = myUtil::normalize(lightDir);
					col += Phong(bestNormal,lightDir,lightVisible(bestCollisionPoint, lightSources[i].direction, numeric_limits<float>::max()), direction, bestColorVec, intensities, n);
				}else{
					//point light
					lightDir = lightSources[i].position - bestCollisionPoint;

					//abnahme der helligkeit mit der distanz
					float dist = myUtil::homogenNorm(lightDir);
					if(dist != 0){
						intensities(3) *= (1.0/dist*dist);
						intensities(4) *= (1.0/dist*dist);
						intensities(5) *= (1.0/dist*dist);
					}
					if(dist != 0){
						intensities(6) *= (1.0/dist*dist);
						intensities(7) *= (1.0/dist*dist);
						intensities(8) *= (1.0/dist*dist);
					}
					lightDir = myUtil::normalize(lightDir);
					col += Phong(bestNormal,lightDir,lightVisible(bestCollisionPoint, lightDir, dist), direction, bestColorVec, intensities, n);
				}
			}
			col *= (1.0f/lightSources.size());
			bestColorVec = col;
	//		cout << bestColor << endl;
		}
	}
	return bestColorVec;
}

bool SceneGraph::lightVisible(CVector<float> point, CVector<float> lightDir, float distToLight){
	CVector<float> bestColor = this->backgroundColor;
	CVector<float> normal;
	CVector<float> collisionPoint;
	float t = distToLight;
	lightDir = myUtil::normalize(lightDir);
	for(int i = 0; i < objects.size(); i++){
		bool collided = false;
		float distance = myUtil::epsi;
		CVector<float> color;
		float a;
		color = objects[i]->collision(point, lightDir, &collided, &distance, &collisionPoint, &normal, true, &a, &a);
		if(collided){
			if(distance < t){
				return false;
			}
		}
	}
	return true;
}

CVector<float> SceneGraph::Phong(CVector<float> normal, CVector<float> lightdirection, bool seeTheLight, CVector<float> viewingRay, CVector<float> colorVec, CVector<float> intensities, float n){
	if(!colorVec.size() == 9)
		cerr << "Phong: this color ist not 9d!" << endl;
	normal = myUtil::normalize(normal);
//	if(normal(0) != normal(0)){//nanPhong
//		return CVector<float>(3,0);
//	}
	lightdirection = myUtil::normalize(lightdirection);
	viewingRay = myUtil::normalize(viewingRay);
	CVector<float> R = 2.0f * (lightdirection * normal) * normal - lightdirection;
	if(myUtil::homogenNorm(R) > 0)
		R = myUtil::normalize(R);
	CVector<float> color = myUtil::color(colorVec(0)*intensities(0), colorVec(1)*intensities(1), colorVec(2)*intensities(2));

	if(seeTheLight){
		//ambientcolor
		//diffusecolor
		//specular = white
		//1color of light
		CVector<float> colorDiffuse = myUtil::color(colorVec(3),colorVec(4),colorVec(5));
		CVector<float> colorSpecular = myUtil::color(colorVec(6),colorVec(7),colorVec(8));
		CVector<float> intensDiffuse = myUtil::color(intensities(3),intensities(4),intensities(5));
		CVector<float> intensSpecular = myUtil::color(intensities(6),intensities(7),intensities(8));
		color += myUtil::elementWiseMulti(intensDiffuse, colorDiffuse) * abs(normal * lightdirection);
		color += myUtil::elementWiseMulti(intensSpecular, colorSpecular) * (float)pow(abs(R * viewingRay), n);
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

BVH* SceneGraph::loadObj(string pathToObj, CVector<float> color, CVector<float> origin, CVector<float> angles, CVector<float> movement){
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
	vector< Triangle* > triangles;

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
	CVector<float> center = movement;
	//delete bvh;

	for(int i = 0; i < triangles.size(); i++){
		triangles[i]->rotate(angles, center);
		//triangles[i]->translate(movement);
	}

	BVH* bvh2 = new BVH(triangles);
	cout << endl;
	objects.push_back(bvh2);
	return bvh2;
}

BVH* SceneGraph::loadObj2(string pathToObj, CVector<float> color, std::vector< CVector<float>  > originVec, int count){
	vector< Triangle* > triangles;
	for(int i = 0; i < count; i++){
		CVector<float> origin = originVec.at(i);
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
	}
	BVH* bvh2 = new BVH(triangles);
	cout << "build one BVH" << endl;
	cout << endl;
	objects.push_back(bvh2);
	return bvh2;
}

CVector<float> SceneGraph::Recursion(CVector<float> color, CVector<float> originPoint, CVector<float> oldViewingDirection, CVector<float> normal, int recursionDepth, float reflection, float transparency){
	if(color.size() != 9)
		cerr << "Recursion: Color doesn't have 9 dimensions!" << endl;
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

CVector<float> SceneGraph::getColorForRay(CVector<float> origin, CVector<float> direction, int recurionDepth){
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
	CVector<float> color(3,0);
	for(int i = 0; i < lightSources.size(); i++){
		CVector<float> lightDir;
		CVector<float> intensities = lightSources[i].intensities;

		if(lightSources.at(i).isDirectionalLight){
			//directional light
			lightDir = lightSources[i].direction;
			lightDir = myUtil::normalize(lightDir);
			bool seeTheLight = lightVisible(pointToEvaluate, lightSources[i].direction, numeric_limits<float>::max());
			color += Phong(normal,lightDir,seeTheLight, direction, col, intensities, n);
		}else{
			//point light
			lightDir = lightSources[i].position - pointToEvaluate;

			//abnahme der helligkeit mit der distanz
			float dist = myUtil::homogenNorm(lightDir);
			if(dist != 0){
				intensities(3) *= (1.0/dist*dist);
				intensities(4) *= (1.0/dist*dist);
				intensities(5) *= (1.0/dist*dist);
			}
			if(dist != 0){
				intensities(6) *= (1.0/dist*dist);
				intensities(7) *= (1.0/dist*dist);
				intensities(8) *= (1.0/dist*dist);
			}
			lightDir = myUtil::normalize(lightDir);
			bool seeTheLight = lightVisible(pointToEvaluate, lightDir, dist);
			color += Phong(normal,lightDir,seeTheLight, direction, col, intensities, n);
		}
	}
	color *= (1.0f/lightSources.size());
	return color;
}

void SceneGraph::loadScene(int scene){
	lightSources = std::vector<Light>(0);
	objects = std::vector<SceneObject*>(0);
	switch(scene){
		case 0:
			//4 balls
			//phong
			//triangle
			//intersetion objects
			//rotation
			//aabb
			//sampling and reconstruction
			//resulution of the image
			//bvh
			inverseCameraMatrix = InverseCameraMatrix(myUtil::PosHom(4.2,2,0), myUtil::PosHom(0,0,0), myUtil::PosHom(0,1,0));
			objects.push_back(new Plane(myUtil::PosHom(4,-4.25,-7), myUtil::PosHom(-4,-4.25,-7), myUtil::PosHom(4,-4.25,7),myUtil::PosHom(0,1,0),myUtil::PosHom(0,0,0),0,0,"white.png"));
			objects.push_back(new Sphere(0.5, myUtil::PosHom(0.7,0.75,0.9), myUtil::color(255, 255, 0), 0, 0));
			objects.push_back(new Sphere(0.5, myUtil::PosHom(0,0.75,1.6), myUtil::color(0, 255, 30),0,0));
			objects.push_back(new Sphere(0.5, myUtil::PosHom(0,0.75,0.5), myUtil::color(255, 0, 0),0,0));
			objects.push_back(new Sphere(1.5, myUtil::PosHom(-1,1.75,-0.5), myUtil::color(255, 0, 255),0,0,"earth.jpg"));
			addLightSource(Light(myUtil::PosHom(-2,4,-1), CVector<float>(4,1), false, myUtil::color9D(0.6,0.6,0.6,0.54,0.54,0.54,0.93,0.93,0.93)));
			addLightSource(Light(myUtil::PosHom(0,1,0), myUtil::PosHom(0,1,0,0), true, myUtil::color9D(0.6,0.6,0.6,0.54,0.54,0.54,0.93,0.93,0.93)));
			break;
		case 1:
			//axis
			inverseCameraMatrix = InverseCameraMatrix(myUtil::PosHom(7,0,0), myUtil::PosHom(0,0,0), myUtil::PosHom(0,1,0));
			addLightSource(Light(myUtil::PosHom(2,0,2), CVector<float>(4,1), false, myUtil::color(0.5,0.5,0.5)));
			addLightSource(Light(myUtil::PosHom(2,4,2), CVector<float>(4,1), false, myUtil::color(0.5,0.5,0.5)));
			objects.push_back(new Sphere(5, myUtil::PosHom(0,0,0), myUtil::color(255, 0, 255),0,0,"lines2.png"));//lines2.png
			break;
		case 2:
		case 3:
			//cool scene
			inverseCameraMatrix = InverseCameraMatrix(myUtil::PosHom(4.2,2,0), myUtil::PosHom(0,0,0), myUtil::PosHom(0,1,0));
			objects.push_back(new Sphere(0.5, myUtil::PosHom(1.0,0.75,1.0), myUtil::color(127, 219, 255), 0, 0.7));
			objects.push_back(new Sphere(0.5, myUtil::PosHom(0.2,0.75,1.7), myUtil::color(0, 255, 30),0,0, "earth.jpg"));
			if(!presentation){
				objects.push_back(new Sphere(0.8, myUtil::PosHom(-0.3,0.9,0.3), myUtil::color(0, 0, 0),0.8,0));
			}else{
				objects.push_back(new Sphere(0.8, myUtil::PosHom(-0.3,0.9,0.3), myUtil::color(255, 128, 0),0.8,0));
			}
			if(!presentation){
				objects.push_back(new Plane(myUtil::PosHom(8,-4.25,-14), myUtil::PosHom(-8,-4.25,-14), myUtil::PosHom(8,-4.25,14),myUtil::PosHom(0,1,0),myUtil::PosHom(0,0,0),0,0.2,"tile.jpg","tile_bump.jpg"));
//				objects.push_back(new Plane(myUtil::PosHom(-8,-4.25,-14), myUtil::PosHom(-8,6.25,-14), myUtil::PosHom(8,-4.25,-14),myUtil::PosHom(0,0,1),myUtil::PosHom(0,0,0),0,0,"wall.jpg", "wall_bump.jpg"));//right
//				objects.push_back(new Plane(myUtil::PosHom(-8,-4.25,14), myUtil::PosHom(-8,6.25,14), myUtil::PosHom(-8,-4.25,-14),myUtil::PosHom(1,0,0),myUtil::PosHom(0,0,0),0,0,"wall.jpg", "wall_bump.jpg"));//back
//				objects.push_back(new Plane(myUtil::PosHom(8,-4.25,14), myUtil::PosHom(8,6.25,14), myUtil::PosHom(-8,-4.25,14),myUtil::PosHom(-1,0,0),myUtil::PosHom(0,0,0),0,0,"wall.jpg", "wall_bump.jpg"));//left
//				objects.push_back(new Plane(myUtil::PosHom(8,-4.25,14), myUtil::PosHom(8,6.25,14), myUtil::PosHom(8,-4.25,-14),myUtil::PosHom(-1,0,0),myUtil::PosHom(0,0,0),0,0,"wall.jpg", "wall_bump.jpg"));//back
//				objects.push_back(new Plane(myUtil::PosHom(8,6.25,14), myUtil::PosHom(-8,6.25,14), myUtil::PosHom(8,6.25,-14),myUtil::PosHom(0,0,-1),myUtil::PosHom(0,0,0),0,0,"ceil.jpg"));
			}else{
				objects.push_back(new Plane(myUtil::PosHom(8,-4.25,-14), myUtil::PosHom(-8,-4.25,-14), myUtil::PosHom(8,-4.25,14),myUtil::PosHom(0,1,0),myUtil::PosHom(0,0,0),0,0.2,"tile.jpg"));
			}
			objects.push_back(new Box(myUtil::PosHom(0,0,0), myUtil::PosHom(4.5,0.5,6.5), myUtil::color(0,255,255),0,0,"wood.jpg"));

			addLightSource(Light(myUtil::PosHom(-0.5,2.7,1.5+0.5*sin(t)), CVector<float>(4,1), false, myUtil::color9D(0.4,0.4,0.4,0.5,0.5,0.5,2,2,2)));
			objects.push_back(new Sphere(0.35, myUtil::PosHom(-0.5,2.7,1.5+0.5*sin(t)), myUtil::color(10000000, 10000000, 10000000), true));
			loadObj("models/lamp3.obj", myUtil::color(255,0,0), myUtil::PosHom(-0.5,2.9,1.5+0.5*sin(t)), myUtil::color(atan((0.5*sin(t))/(2.0)),0,0), myUtil::PosHom(-0.5,2.7,1.5+0.5*sin(t)));
			objects.push_back(new Box(myUtil::PosHom(-0.5,4.7,1.5), myUtil::PosHom(0.05,3.0,0.05), myUtil::color((0.5*sin(t))/(2.0),0,0),0,0,"white.png"));
			objects[objects.size()-1]->rotate(myUtil::color(atan((0.5*sin(t))/(2.0)),0,0));

			objects.push_back(new Cylinder(myUtil::PosHom(1.8,-4.25,2.8), 4, myUtil::color(50,50,50),0.3));
			objects.push_back(new Cylinder(myUtil::PosHom(-1.8,-4.25,2.8), 4, myUtil::color(50,50,50),0.3));
			objects.push_back(new Cylinder(myUtil::PosHom(1.8,-4.25,-2.8), 4, myUtil::color(50,50,50),0.3));
			objects.push_back(new Cylinder(myUtil::PosHom(-1.8,-4.25,-2.8), 4, myUtil::color(50,50,50),0.3));
			addLightSource(Light(myUtil::PosHom(-0.5,-1,1.5), CVector<float>(4,1), false, myUtil::color9D(0.45,0.45,0.45,1.6,1.6,1.6,1.6,1.6,1.6)));
			if(scene == 3){
				//cool scene + duck
				loadObj("models/ducky2.obj", myUtil::color(255,255,0), myUtil::PosHom(0.5,0.25,-1));
				objects.push_back(new Sphere(0.06,myUtil::PosHom(0.998662,2.10338,-0.498035), myUtil::color(0,0,0),0,0));
				objects.push_back(new Sphere(0.06,myUtil::PosHom(0.002,2.10338,-0.498035), myUtil::color(0,0,0),0,0));
			}
		break;
		case 4:
			inverseCameraMatrix = InverseCameraMatrix(myUtil::PosHom(4.2,2,0), myUtil::PosHom(0,0,0), myUtil::PosHom(0,1,0));
			addLightSource(Light(myUtil::PosHom(-0.5,-1,1.5), CVector<float>(4,1), false, myUtil::color(0.9,0.9,0.9)));
			addLightSource(Light(myUtil::PosHom(-0.5,2.7,1.5), CVector<float>(4,1), false, myUtil::color(0.9,0.9,0.9)));
			objects.push_back(new Plane(myUtil::PosHom(8,-4.25,-14), myUtil::PosHom(-8,-4.25,-14), myUtil::PosHom(8,-4.25,14),myUtil::PosHom(0,1,0),myUtil::PosHom(0,0,0),0,0,"tile.jpg","tile_bump.jpg"));
			break;
		case 5:
			inverseCameraMatrix = InverseCameraMatrix(myUtil::PosHom(0.3,0,0), myUtil::PosHom(0,0,0), myUtil::PosHom(0,1,0));
			addLightSource(Light(myUtil::PosHom(-0.5,2.7,0.0+0.5*sin(t)), CVector<float>(4,1), false, myUtil::color9D(0.4,0.4,0.4,0.5,0.5,0.5,2,2,2)));
			objects.push_back(new Sphere(0.35, myUtil::PosHom(0,0.2,0.2), myUtil::color(255,0,0),0,0,"lines2.png"));
			break;
		case 6:
		{
			inverseCameraMatrix = InverseCameraMatrix(myUtil::PosHom(5,0,0), myUtil::PosHom(0,0,0), myUtil::PosHom(0,1,0));
			addLightSource(Light(myUtil::PosHom(5.2,0,0), CVector<float>(4,1), false, myUtil::color9D(0.5,0.5,0.5,0.4,0.4,0.4,0.1,0.1,0.1)));
			std::vector< CVector<float> > origins;
			origins.push_back(myUtil::PosHom(0,2.5,2.5));
			origins.push_back(myUtil::PosHom(0,-2.5,2.5));
			origins.push_back(myUtil::PosHom(0,2.5,-2.5));
			origins.push_back(myUtil::PosHom(0,-2.5,-2.5));
			loadObj2("models/sphere100x100.obj", myUtil::color(255,255,0), origins,1);
		}
		break;
		{
		case 7:
			inverseCameraMatrix = InverseCameraMatrix(myUtil::PosHom(5,0,0), myUtil::PosHom(0,0,0), myUtil::PosHom(0,1,0));
			addLightSource(Light(myUtil::PosHom(5.2,0,0), CVector<float>(4,1), false, myUtil::color9D(0.5,0.5,0.5,0.4,0.4,0.4,0.1,0.1,0.1)));
			std::vector< CVector<float> > origins;
			origins.push_back(myUtil::PosHom(0,2.5,2.5));
			origins.push_back(myUtil::PosHom(0,-2.5,2.5));
			origins.push_back(myUtil::PosHom(0,2.5,-2.5));
			origins.push_back(myUtil::PosHom(0,-2.5,-2.5));
			loadObj2("models/sphere100x100.obj", myUtil::color(255,255,0), origins,2);
		}
		break;
		case 8:
		{
			inverseCameraMatrix = InverseCameraMatrix(myUtil::PosHom(5,0,0), myUtil::PosHom(0,0,0), myUtil::PosHom(0,1,0));
			addLightSource(Light(myUtil::PosHom(5.2,0,0), CVector<float>(4,1), false, myUtil::color9D(0.5,0.5,0.5,0.4,0.4,0.4,0.1,0.1,0.1)));
			std::vector< CVector<float> > origins;
			origins.push_back(myUtil::PosHom(0,2.5,2.5));
			origins.push_back(myUtil::PosHom(0,-2.5,2.5));
			origins.push_back(myUtil::PosHom(0,2.5,-2.5));
			origins.push_back(myUtil::PosHom(0,-2.5,-2.5));
			loadObj2("models/sphere100x100.obj", myUtil::color(255,255,0), origins,4);
		}
		break;
		case 9:
			inverseCameraMatrix = InverseCameraMatrix(myUtil::PosHom(5,0,0), myUtil::PosHom(0,0,0), myUtil::PosHom(0,1,0));
			addLightSource(Light(myUtil::PosHom(5.2,0,0), CVector<float>(4,1), false, myUtil::color9D(0.5,0.5,0.5,0.4,0.4,0.4,0.1,0.1,0.1)));
			addLightSource(Light(myUtil::PosHom(-0.5,-1,1.5), CVector<float>(4,1), false, myUtil::color(0.9,0.9,0.9)));
			addLightSource(Light(myUtil::PosHom(-0.5,2.7,1.5), CVector<float>(4,1), false, myUtil::color(0.9,0.9,0.9)));
			objects.push_back(new Sphere(1.4, myUtil::PosHom(0.6,0,-2), myUtil::color(255, 0, 0), 0,0.8,"earth.jpg"));
			objects.push_back(new Sphere(1.6, myUtil::PosHom(-0.6,1.75,-0.5), myUtil::color(255, 128, 0),0.8,0));
			objects.push_back(new Sphere(1.5, myUtil::PosHom(0.1,0.9,2.3), myUtil::color(255, 0, 255),0,0,"earth.jpg"));
			objects.push_back(new Plane(myUtil::PosHom(8,-4.25,-14), myUtil::PosHom(-8,-4.25,-14), myUtil::PosHom(8,-4.25,14),myUtil::PosHom(0,1,0),myUtil::PosHom(0,0,0),0,0.4,"tile.jpg"));
		break;
		case 10:
			inverseCameraMatrix = InverseCameraMatrix(myUtil::PosHom(5,4,2), myUtil::PosHom(0,0,0), myUtil::PosHom(0,1,0));
			addLightSource(Light(myUtil::PosHom(5.2,0,0), CVector<float>(4,1), false, myUtil::color9D(0.5,0.5,0.5,0.4,0.4,0.4,0.1,0.1,0.1)));
			addLightSource(Light(myUtil::PosHom(-0.5,4.7,1.5), CVector<float>(4,1), false, myUtil::color(0.9,0.9,0.9)));
			objects.push_back(new Box(myUtil::PosHom(0,0.5,-5.5), myUtil::PosHom(12,1,1), myUtil::color(1,1,12),0,0,"wood.jpg"));
			objects.push_back(new Box(myUtil::PosHom(5.5,0.5,0), myUtil::PosHom(1,1,10), myUtil::color(1,12,1),0,0,"wood.jpg"));
			objects.push_back(new Box(myUtil::PosHom(0,0.5,5.5), myUtil::PosHom(12,1,1), myUtil::color(1,1,12),0,0,"wood.jpg"));
			objects.push_back(new Box(myUtil::PosHom(-5.5,0.5,0), myUtil::PosHom(1,1,10), myUtil::color(1,12,1),0,0,"wood.jpg"));
			objects.push_back(new Plane(myUtil::PosHom(6,-0,6), myUtil::PosHom(6,-0,-6), myUtil::PosHom(-6,-0,6),myUtil::PosHom(0,1,0),myUtil::PosHom(0,0,0),0,0,"wood.jpg"));
			objects.push_back(new Plane(myUtil::PosHom(5.75,0.75,5.75), myUtil::PosHom(5.75,0.75,-5.75), myUtil::PosHom(-5.75,0.75,5.75),myUtil::PosHom(0,1,0),myUtil::PosHom(0,0,255),0.3,0.3,"water.jpg"));

			loadObj("models/ducky2.obj", myUtil::color(255,255,0), myUtil::PosHom(1.5,0.75,-1.5));
			objects.push_back(new Sphere(0.06,myUtil::PosHom(0.998662+1,2.10338+0.5,-0.498035-0.5), myUtil::color(0,0,0),0,0));
			objects.push_back(new Sphere(0.06,myUtil::PosHom(0.002+1,2.10338+0.5,-0.498035-0.5), myUtil::color(0,0,0),0,0));

			objects.push_back(new Sphere(0.8, myUtil::PosHom(2,0.75,1.5), myUtil::color(255, 0, 0),0,0.7));
			objects.push_back(new Sphere(1.7, myUtil::PosHom(-2.5,1.25,2), myUtil::color(255, 0, 255),0,0,"earth.jpg"));
		break;
	}
}

