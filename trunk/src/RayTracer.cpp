/*
 * RayTracer.cpp
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#include "RayTracer.h"
#include <qpixmap.h>
#include <iostream>
#include "mathe/CMatrix.h"
#include "SceneGraph.h"
#include <limits>
#include <math.h>
#include "myUtil.h"
#include <cmath>

using namespace std;

RayTracer::RayTracer(QPixmap* _img) {
	img = _img;
	graph = new SceneGraph();
	image = QImage(width,height,QImage::Format_ARGB32);
	bottom = 0.48*2.5;
	right = 0.64*2.5;
	near = 1;
	far = 10;
}

RayTracer::~RayTracer() {
	delete graph;
}

void RayTracer::debug(){
	//Q_EMIT(seeTheLightMap());
	graph->loadObj("sphere.obj", myUtil::color(255,255,0));//kleinbottle
	std::cout << "debug" << std::endl;
}

void RayTracer::draw(){
	//image = QImage(width,height,QImage::Format_ARGB32);
	//image.fill(qRgba(200,200,255,255));
	cout << "Rendering started!" << endl;
	CMatrix<float> cameraMatrix = graph->getCameraMatrix();
	CVector<float> origin = myUtil::PosHom(0,0,0);
	origin = cameraMatrix * origin;
	//origin *= -1;
	for(int y = 0; y < height; y++){
		if(y%(height/10) == 0){
			if(y == 0){
				cout << "  0% finished" << endl;
			}else{
				cout << " " << (int)(y/(height/10))*10 << "% finished" << endl;
			}
		}
		for(int x = 0; x < width; x++){
			CVector<float> dir(4,0);
			//+0.5 damit in mitte des pixels
//			dir(0) = (((float)x+0.5)/((float)width))*2*abs(right)-abs(right);
//			dir(1) = -((((float)y+0.5)/((float)height))*2*abs(bottom)-abs(bottom));
			dir(0) = -abs(right) +(((float)x+0.5)/((float)width)) *2*abs(right);
			dir(1) = abs(bottom)-(((float)y+0.5)/((float)height))*2*abs(bottom);
			dir(2) = -near;
//			if(x == 320 && y == 240)
//				cout << "dir: " << dir << endl;
			//dir = myUtil::normalize(dir);
			dir = cameraMatrix * dir;
			dir = myUtil::normalize(dir);
			//dir *= -1.0f;
//			if(x == 320 && y == 240)
//				cout << "Mat * dir: " << dir << endl;
			//cout << dir << endl;

			CVector<float> col = graph->castRay(origin, dir);
			QColor color = QColor(min((int)col(0),255),min((int)col(1),255),min((int)col(2),255),255);
			image.setPixel(x,y,color.rgba());
		}
	}
	cout << "100% finished" << endl;
	cout << "Rendering finised!" << endl;
	//img->fill(QColor(0,0,255,255));
	img->convertFromImage(image);
	image.save("test3.png","png");
	//img->fill(QColor(0,0,255,255))
	Q_EMIT(repaint());
}

void RayTracer::seeTheLightMap(){
	CMatrix<float> cameraMatrix = graph->getCameraMatrix();
	CVector<float> origin = myUtil::PosHom(0,0,0);
	origin = cameraMatrix * origin;
	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
//	int y = 334;
//	int x = 448;
			CVector<float> dir(4,0);
			dir(0) = -abs(right) +(((float)x+0.5)/((float)width)) *2*abs(right);
			dir(1) = abs(bottom)-(((float)y+0.5)/((float)height))*2*abs(bottom);
			dir(2) = -near;
			dir = cameraMatrix * dir;
			dir = myUtil::normalize(dir);
			CVector<float> col = graph->castLightRay(origin, dir);
			QColor color = QColor(min((int)col(0),255),min((int)col(1),255),min((int)col(2),255),255);
			image.setPixel(x,y,color.rgba());
		}
	}
	img->convertFromImage(image);
	image.save("lightMap.png","png");
	Q_EMIT(repaint());
}
