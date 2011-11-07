/*
 * SceneGraph.cpp
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#include "SceneGraph.h"
#include <iostream>
#include "MyVector.h"
#include "Sphere.h"
#include "myUtil.h"
#include <cmath>
#include <limits>
#include "Triangle.h"
#include <fstream>
#include <iostream>
#include <QString>
#include <QObject>
#include <QStringList>

using namespace std;

SceneGraph::SceneGraph(){
	lightSources = std::vector<Light>(0);
	objects = std::vector<SceneObject*>(0);
	inverseCameraMatrix = InverseCameraMatrix(myUtil::PosHom(3,3,3), myUtil::PosHom(0,0,0), myUtil::PosHom(0,1,0));
	//setCameraMatrix(MyVector(3,0),1,10,4,6);
	backgroundColor = myUtil::color(127.0f,219.0f,255.0f);
	EAmbient = myUtil::color(200,100,50);

	addLightSource(Light(myUtil::PosHom(2,4,-3), MyVector(4,1), false, myUtil::color(0.5,0.5,0)));
	addLightSource(Light(myUtil::PosHom(-2,4,-3), MyVector(4,1), false, myUtil::color(0.5,0.5,0)));
	addLightSource(Light(myUtil::PosHom(1,1,1), MyVector(4,1), false, myUtil::color(0.5,0.5,0)));
	addLightSource(Light(myUtil::PosHom(1,1,0), myUtil::PosHom(1,1,1,0), true, myUtil::color(0.5,0.5,0)));

	intenseAmbient = myUtil::color(0,100,100);
	intenseDiffuse = myUtil::color(100,100,100);
	intenseSpecular = myUtil::color(250,250,250);
	n = 5;

//	MyVector pos = myUtil::PosHomogen(0,6,-1);
//	pos = inverseCameraMatrix * pos;
//	cout << pos << endl;
//	MyVector col = myUtil::Pos(255,0,0);
//	Sphere* s = new Sphere(2.0,pos,col);
//	objects.push_back(s);
//	MyVector pos1 = myUtil::PosHomogen(2,4,0);
//	pos1 = inverseCameraMatrix * pos1;
//	MyVector col1 = myUtil::Pos(255,255,0);
//	Sphere* s1 = new Sphere(1.1,pos1,col1);
//	objects.push_back(s1);
//	MyVector pos2 = myUtil::PosHomogen(-1,7,-2);
//	pos2 = inverseCameraMatrix * pos2;
//	MyVector col2 = myUtil::Pos(0,0,255);
//	Sphere* s2 = new Sphere(2.5,pos2,col2);
//	objects.push_back(s2);
	{MyVector pos = myUtil::PosHom(1,2,3);
	//	 pos = inverseCameraMatrix * pos;
		 MyVector col = myUtil::color(0,0,0);
		 Sphere* s = new Sphere(0.1, pos, col);
	objects.push_back(s);}
	//koordinatensystem
	{MyVector pos = myUtil::PosHom(0,0,0);
//	 pos = inverseCameraMatrix * pos;
	 MyVector col = myUtil::color(255, 255, 255);
	 Sphere* s = new Sphere(0.7, pos, col);
	 objects.push_back(s);
	 }
	for(int i = 1; i < 4; i++){
		Sphere* s;
		MyVector pos;
		MyVector col;

		{pos = myUtil::PosHom(i,0,0);
//		pos = inverseCameraMatrix * pos;
		col = myUtil::color(255, 0, 0);
		s = new Sphere(0.4, pos, col);
		objects.push_back(s);}

		{MyVector pos = myUtil::PosHom(i,i,0);
		MyVector col = myUtil::color(255, 255, 0);
		Sphere* s = new Sphere(0.2, pos, col);
		objects.push_back(s);}

		{pos = myUtil::PosHom(0, i, 0);
		col = myUtil::color(0, 255, 0);
		s = new Sphere(0.4, pos, col);
		objects.push_back(s);}

		{MyVector pos = myUtil::PosHom(i,0,i);
		MyVector col = myUtil::color(255, 0, 255);
		Sphere* s = new Sphere(0.2, pos, col);
		objects.push_back(s);}

		pos = myUtil::PosHom(0, 0, i);
//		pos = inverseCameraMatrix * pos;
		col = myUtil::color(0, 0, 255);
		s = new Sphere(0.4, pos, col);
		objects.push_back(s);

		{MyVector pos = myUtil::PosHom(0,i,i);
		MyVector col = myUtil::color(0, 255, 255);
		Sphere* s = new Sphere(0.2, pos, col);
		objects.push_back(s);}

	}
	Triangle* tr = new Triangle(myUtil::PosHom(1,0,0), myUtil::PosHom(0,1,0), myUtil::PosHom(0,0,1), myUtil::PosHom(1,1,1), myUtil::color(255,255,0));
	objects.push_back(tr);
}

SceneGraph::SceneGraph(MyMatrix _cameraMatrix, MyVector _backgroundColor){
	lightSources = std::vector<Light>(0);
	objects = std::vector<SceneObject*>(0);
	cameraMatrix = _cameraMatrix;
	backgroundColor = _backgroundColor;
}

SceneGraph::~SceneGraph() {

}

MyMatrix SceneGraph::InverseCameraMatrix(MyVector cameraPos, MyVector lookAt, MyVector up){
//	//nach: http://www.opengl.org/sdk/docs/man/xhtml/gluLookAt.xml
//	MyMatrix mat(4,4,0);
//	MyVector direction = lookAt - cameraPos;
//	direction *= (1.0/direction.norm());
//	up *= (1.0/up.norm());
//	MyVector side = direction/up;//cross product
//	side *= (1.0/side.norm());
//	MyVector u = side/direction;
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
	MyMatrix mat(4,4,0);

	//calculate view-direction
	MyVector view = lookAt - cameraPos;
	view = myUtil::normalize(view);

	//calculate vector to side
	MyVector side = view / up;
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

	mat.print();

	return mat;
}

void SceneGraph::addLightSource(Light light){
	MyVector pos = light.position;
	MyVector col = myUtil::color(255,255,0);
	Sphere* s = new Sphere(0.1,pos,col,true);
//	objects.push_back(s);
	lightSources.push_back(light);
}

void SceneGraph::addSceneObject(SceneObject* object){
	objects.push_back(object);
}

void SceneGraph::setCameraMatrix(MyMatrix _cameraMatrix){
	cameraMatrix = _cameraMatrix;
}

MyMatrix SceneGraph::ProjectionMatrix(MyVector _origin, float near, float far, float width, float height){
//	origin = _origin;

	cameraMatrix = MyMatrix(4,4,0);
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


MyVector SceneGraph::getColor(MyVector _origin, MyVector direction){
	MyVector color = backgroundColor;
	//TODO get collisions und ersetzte Farbe wenn collision
}

MyVector SceneGraph::castRay(MyVector origin, MyVector direction){
	MyVector bestColor = this->backgroundColor;
	MyVector normal;
	MyVector collisionPoint;
	MyVector bestNormal;
	MyVector bestCollisionPoint;
	float t = std::numeric_limits<float>::max();
	bool hit = false;

	for(int i = 0; i < objects.size(); i++){
		bool collided = false;
		float distance = 0;
		MyVector color = objects[i]->collision(origin, direction, &collided, &distance, &collisionPoint, &normal, false);
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
		MyVector col(3,0);
		for(int i = 0; i < lightSources.size(); i++){
			MyVector EDiffuse = lightSources[i].IDiffuse;
			MyVector ESpecular = lightSources[i].ISpecular;
			MyVector lightDir;

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
	return bestColor;
}

bool SceneGraph::lightVisible(MyVector point, MyVector lightDir, float distToLight){
//	if(directionalLight)
//		return true;
	MyVector bestColor = this->backgroundColor;
	MyVector normal;
	MyVector collisionPoint;
//	MyVector bestNormal;
//	MyVector bestCollisionPoint;
	float t = distToLight;
//	bool hit = false;
	for(int i = 0; i < objects.size(); i++){
		bool collided = false;
		float distance = myUtil::epsi;
		MyVector color;
		color = objects[i]->collision(point, myUtil::normalize(lightDir), &collided, &distance, &collisionPoint, &normal, true);
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

MyVector SceneGraph::Phong(MyVector normal, MyVector lightdirection, bool seeTheLight, MyVector viewingRay, MyVector EDiffuse, MyVector ESpecular, float n){
	normal = myUtil::normalize(normal);
	lightdirection = myUtil::normalize(lightdirection);
	viewingRay = myUtil::normalize(viewingRay);
	cerr << "graph::327" << endl;
	MyVector R;
//	MyVector R = 2.0f * (lightdirection * normal) * normal - lightdirection;
	if(myUtil::homogenNorm(R) > 0)
		R = myUtil::normalize(R);
	MyVector color(3,0);

	if(seeTheLight){
		color += myUtil::elementWiseMulti(intenseDiffuse, EDiffuse) * abs(normal * lightdirection);
		color += myUtil::elementWiseMulti(intenseSpecular, ESpecular) * (float)pow(abs(R * viewingRay), n);
	}
//	cout << color <<endl;
	return color;
}

MyMatrix SceneGraph::Rx(float angle){
	MyMatrix mat(4,4,0);
	mat(0,0) = 1.0;
	mat(1,1) = cos(angle);
	mat(1,2) = -sin(angle);
	mat(2,1) = sin(angle);
	mat(2,2) = cos(angle);
	mat(3,3) = 1.0;

	return mat;
}

MyMatrix SceneGraph::Ry(float angle){
	MyMatrix mat(4,4,0);
	mat(0,0) = cos(angle);
	mat(0,2) = sin(angle);
	mat(2,0) = -sin(angle);
	mat(2,2) = cos(angle);
	mat(1,1) = 1.0;
	mat(3,3) = 1.0;

	return mat;
}

MyMatrix SceneGraph::Rz(float angle){
	MyMatrix mat(4,4,0);
	mat(0,0) = cos(angle);
	mat(0,1) = -sin(angle);
	mat(2,2) = 1.0;
	mat(1,0) = sin(angle);
	mat(1,1) = cos(angle);
	mat(3,3) = 1.0;

	return mat;
}

MyMatrix SceneGraph::getCameraMatrix(){
	MyMatrix mat = inverseCameraMatrix;
	mat.inv();
//	cout << "inverse Mat: " << endl << mat << endl;
	return mat;
}

MyVector SceneGraph::castLightRay(MyVector origin, MyVector direction){
	MyVector bestColor = this->backgroundColor;
	MyVector normal;
	MyVector collisionPoint;
	MyVector bestNormal;
	MyVector bestCollisionPoint;
	float t = 9999999999999;
	bool hit = false;

	for(int i = 0; i < objects.size(); i++){
		bool collided = false;
		float distance = 0;
		MyVector color = objects[i]->collision(origin, direction, &collided, &distance, &collisionPoint, &normal, false);
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
		MyVector col(3,0);
		for(int i = 0; i < lightSources.size(); i++){
			MyVector lightDir = lightSources[i].position - bestCollisionPoint;
			if(lightSources[i].isDirectionalLight){
				MyVector dir = lightSources[i].direction;
//				dir *= -1.0;
				if(lightVisible(bestCollisionPoint,dir,numeric_limits<float>::max())){
					col += myUtil::color(255,255,255);
				}
			}else{
				MyVector dir = lightSources[i].position - bestCollisionPoint;
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

kdTree* SceneGraph::loadObj(string pathToObj, MyVector color){
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

	vector< MyVector > vertices;
	vertices.push_back(MyVector(3,0));//damit ab 1 gezählt werden kann
	vector< MyVector > normals;
	normals.push_back(MyVector(3,0));//damit ab 1 gezählt werden kann
	vector< SceneObject* > trinagles;

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
//				cout << list.at(j).toStdString().c_str() << endl;
				nums[ind] = list.at(j).toFloat();
				ind++;
			}
			vertices.push_back(myUtil::color(nums[0]+1,nums[1],nums[2]));
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
//				cout << list.at(j).toStdString().c_str() << endl;
				nums[ind] = list.at(j).toFloat();
				ind++;
			}
			normals.push_back(myUtil::color(nums[0],nums[1],nums[2]));
		}

		//triangle
		if(text.at(i)[0] == 'f'){
			QStringList list = text.at(i).split(" ");
			int verts[3];
			int norms[3];
			int ind = 0;
			for(int j = 1; j < list.size() && ind < 3; j++){
				if(list.at(j).toStdString() == " " || list.at(j).toStdString() == "  " || list.at(j).size() == 0)
					continue;
				QStringList list2 = list.at(j).split("/");
				verts[ind] = list2[0].toInt();
				norms[ind] = list2[2].toInt();
//				cout << verts[ind] << " " << norms[ind] << endl;
				++ind;
			}
			MyVector normal = normals.at(norms[0]) + normals.at(norms[1]) + normals.at(norms[2]);
//			cout << normal << endl;
			normal *= (1.0/3.0);
			normal *= (1.0/normal.norm());
			//objects.push_back(new Triangle(vertices.at(verts[0]), vertices.at(verts[1]), vertices.at(verts[2]), normal, color));
			trinagles.push_back(new Triangle(vertices.at(verts[0]), vertices.at(verts[1]), vertices.at(verts[2]), normal, color));
		}
	}

	kdTree* back = new kdTree(trinagles);

	objects.push_back(back);
	return back;
}
