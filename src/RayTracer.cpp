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
#include <time.h>

using namespace std;

RayTracer::RayTracer(QPixmap* _img) {
	img = _img;
	graph = new SceneGraph();
	image = QImage(width,height,QImage::Format_ARGB32);
	bottom = 0.48*2.5;
	right = 0.64*2.5;
	near = 1;
	far = 10;
	srand ( time(NULL) );
}

RayTracer::~RayTracer() {
	delete graph;
}

void RayTracer::debug(){
	Q_EMIT(seeTheLightMap());
	for(int i = 0; i < 10; i++){
		cout << i << ": " << HammersleyValue(i,5) << "\t\t";
		cout << endl;
	}
	//graph->loadObj("sphere.obj", myUtil::color(255,255,0));//kleinbottle
	std::cout << "debug" << std::endl;
}

void RayTracer::draw(){
	//image = QImage(width,height,QImage::Format_ARGB32);
	//image.fill(qRgba(200,200,255,255));
	cout << "Rendering started!" << endl;
	cameraMatrix = graph->getCameraMatrix();
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
			//#############################################
			CVector<float> col = Sample(x, y, 'n', 2, 'm');

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
			dir(0) =-abs(right) +(((float)x+0.5)/((float)width)) *2*abs(right);
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

CVector<float> RayTracer::Sample(int x, int y, char kindOfSampling, int sampleCount, char kindOfReconstruction, float minDist, int p1, int p2){
	CVector<float> origin = myUtil::PosHom(0,0,0);
	origin = cameraMatrix * origin;
	vector< CVector<float> > col(sampleCount*sampleCount);
	CVector<float> dir(4,0);
	CVector<float> pixelVal;
	float strataSize;

	float xPix;
	float yPix;

	switch (kindOfSampling) {
		case 'n':
			//standart sampling one ray per center of pixel
			xPix = ((float)x) + 0.5;
			yPix = ((float)y) + 0.5;
			dir = myUtil::normalize(cameraMatrix * myUtil::PosHom(-abs(right) +((xPix+0.5)/((float)width)) *2*abs(right), abs(bottom)-((yPix+0.5)/((float)height))*2*abs(bottom), -near, 0));
			return graph->castRay(origin, dir);
			break;
		case 'r':
			//random sampling
			for(int i = 0; i < sampleCount*sampleCount; i++){
				xPix = ((float)x) + ((rand() % 1000)/1000.0);//value between 0 and 1
				yPix = ((float)y) + ((rand() % 1000)/1000.0);
				dir = myUtil::normalize(cameraMatrix * myUtil::PosHom(-abs(right) +((xPix+0.5)/((float)width)) *2*abs(right), abs(bottom)-((yPix+0.5)/((float)height))*2*abs(bottom), -near, 0));
				pixelVal = graph->castRay(origin, dir);
				col.at(i) = myUtil::Pos5D(pixelVal(0), pixelVal(1), pixelVal(2), xPix, yPix);
			}
			return Reconstruct(col, kindOfReconstruction);
			break;
		case 's':
			//stratified sampling
			strataSize = 1.0/((float)sampleCount);
			for(int i = 0; i < sampleCount; i++){//x
				for(int j = 0; j < sampleCount; j++){//y
					xPix = x + ((float)i)*strataSize;
					yPix = y + ((float)j)*strataSize;
					xPix += (rand() % 1000)/(1000.0*((float)sampleCount));//value between 0 and 1/strataSize
					yPix += (rand() % 1000)/(1000.0*((float)sampleCount));
					dir = myUtil::normalize(cameraMatrix * myUtil::PosHom(-abs(right) +((xPix+0.5)/((float)width)) *2*abs(right), abs(bottom)-((yPix+0.5)/((float)height))*2*abs(bottom), -near, 0));
					pixelVal = graph->castRay(origin, dir);
					col.at(i*sampleCount+j) = myUtil::Pos5D(pixelVal(0), pixelVal(1), pixelVal(2), xPix, yPix);
				}
			}
			return Reconstruct(col, kindOfReconstruction);
			break;
		case 'p':
			//poisson sampling it is cheaper
			for(int i = 0; i < sampleCount*sampleCount;){
				up:
				xPix = ((float)x) + ((rand() % 1000)/1000.0);//value between 0 and 1
				yPix = ((float)y) + ((rand() % 1000)/1000.0);
				for(int j = 0; j < i; j++){
					if((xPix - col.at(j)(3))*(xPix - col.at(j)(3)) + (yPix - col.at(j)(4))*(yPix - col.at(j)(4)) < minDist)
						goto up;//sorry!
				}
				//take this sample
				dir = myUtil::normalize(cameraMatrix * myUtil::PosHom(-abs(right) +((xPix+0.5)/((float)width)) *2*abs(right), abs(bottom)-((yPix+0.5)/((float)height))*2*abs(bottom), -near, 0));
				pixelVal = graph->castRay(origin, dir);
				col.at(i) = myUtil::Pos5D(pixelVal(0), pixelVal(1), pixelVal(2), xPix, yPix);
				i++;
			}
			return Reconstruct(col, kindOfReconstruction);
			break;
		case 'h':
			//halton sampling
			for(int i = 0; i < sampleCount*sampleCount; i++){
				xPix = ((float)x) + HammersleyValue(i,p1);//value between 0 and 1
				yPix = ((float)y) + HammersleyValue(i,p2);
				dir = myUtil::normalize(cameraMatrix * myUtil::PosHom(-abs(right) +((xPix+0.5)/((float)width)) *2*abs(right), abs(bottom)-((yPix+0.5)/((float)height))*2*abs(bottom), -near, 0));
				pixelVal = graph->castRay(origin, dir);
				col.at(i) = myUtil::Pos5D(pixelVal(0), pixelVal(1), pixelVal(2), xPix, yPix);
			}
			return Reconstruct(col, kindOfReconstruction);
			break;
		default:
			cerr << "kindOfSampling: \'n\' = center of Pixel(standart)\n                \'r\' = random\n                \'s\' = stratified\n                \'p\' = poisson\n                \'h\' = halton" << endl;
			break;
	}
	return CVector<float>(3,0);
}

CVector<float> RayTracer::Reconstruct(vector< CVector<float> > col, char kindOfReconstruction){
	//vectorelements of col are like: [red,green,blue,xpos,ypos]
	CVector<float> color(3,0);
	CVector<float> pixValue;
	float distSum = 0;

	switch (kindOfReconstruction) {
		case 'b':
			//boxfilter = mean
			for(int i = 0; i < col.size(); i++){
				color += myUtil::PosHom(col.at(i)(0),col.at(i)(1),col.at(i)(2));
			}
			color *= (1.0/col.size());
			return color;
			break;
		case 'm':
			//mitchell = gauß
			for(int i = 0; i < col.size(); i++){
				pixValue = myUtil::PosHom(col.at(i)(0),col.at(i)(1),col.at(i)(2));
				int px = col.at(i)(3);
				int py = col.at(i)(4);
				float xDist = (col.at(i)(3) -((float)px))-0.5;//signed
				float yDist = (col.at(i)(4) -((float)py))-0.5;
				float weight = gauss(sqrt(xDist*xDist + yDist*yDist));//Gewichtung mit Abstand
				pixValue *= weight;
				distSum += weight;
				color +=  pixValue;
			}
			color *= (1.0/distSum);//Normalisierung
			return color;
			break;
		default:
			cerr << "kindOfReconstruction: \'b\' = box(standart)\n                      \'m\' = mitchell" << endl;
			break;
	}
	return CVector<float>(3,0);
}

float RayTracer::gauss(float dist){
	return (1.0/sqrt(2.0*3.14159))*exp(-0.5*dist);
}

float RayTracer::HammersleyValue(int k, int p){
	int p_prime = p;
	int k_prime = k;
	float phi = 0;
	while(k_prime > 0){
		float a = k_prime % p;
		phi += a/p_prime;
		k_prime /= p;
		p_prime *= p;
	}
	return phi;
}
