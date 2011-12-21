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
#include "Sphere.h"
#include <limits>
#include <math.h>
#include "myUtil.h"
#include <cmath>
#include <time.h>
#include <sys/time.h>

using namespace std;

RayTracer::RayTracer() {
	presentation = true;
}

RayTracer::RayTracer(QPixmap* _img, bool pres, int _width, int _height, int argc, char *argv[]) {
	presentation = pres;
//	mutex = QMutex();
	graph = new SceneGraph(presentation, argc, argv);
	height = _height+1;
	width = _width;
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
//	Q_EMIT(seeTheLightMap());
//	for(int i = 0; i < 10; i++){
//		cout << i << ": " << HammersleyValue(i,5) << "\t\t";
//		cout << endl;
//	}
	std::cout << "debug" << std::endl;
}

#define CHUNKSIZE 1
static struct timeval start_time;

void RayTracer::run(){
	for(int i = 0; i < 5; i++){
		for(int j = 0; j < 5; j++){
			cout << HammersleyValue(i,5) << " " << HammersleyValue(j,5) << endl;
		}
	}
	gettimeofday(&start_time, NULL);
	//image = QImage(width,height,QImage::Format_ARGB32);
	//image.fill(qRgba(200,200,255,255));
	cout << "Rendering started!" << endl;
	cameraMatrix = graph->getCameraMatrix();
	//origin *= -1;

//	CVector<float> col = Sample(378, 250, 'n', 2, 'm');
	cout << "Drawing image with " << sampling << " " << reconstruction << " and " << rayCount*rayCount << " rays per pixel" << endl;

	CVector<float> dx;
	CVector<float> dy;
	SamplePositions(sampling, rayCount, &dx, &dy);

	#pragma omp parallel for schedule(dynamic, CHUNKSIZE)

	for(int y = 0; y < height-1; y++){
		#pragma omp critical
		{
			mutex.lock();
			image2 = image;
			mutex.unlock();
		}
		
		if(y%(height/10) == 0){
			Q_EMIT(repaint());
			if(y == 0){
				#pragma omp critical
				{
					Q_EMIT(setProgress(0));
					cout << "  0% finished" << endl;
				}
			}else{
				#pragma omp critical
				{
					cout << " " << (int)(y/(height/10))*10 << "% finished" << endl;
					Q_EMIT(setProgress((int)(y/(height/10))*10));
				}
			}
		}
		for(int x = 0; x < width; x++){
			//#############################################
			CVector<float> col;
//			if(presentation){
//				col = SampleFix(x, y, sampling, rayCount, reconstruction,dx,dy);
//			}else{
				col = Sample(x, y, sampling, rayCount, reconstruction);
//			}

			QColor color = QColor(min((int)col(0),255),min((int)col(1),255),min((int)col(2),255),255);
			#pragma omp critical
				image.setPixel(x,y,color.rgba());
		}
	}

	#pragma omp critical
	{
		cout << "100% finished" << endl;
		Q_EMIT(setProgress(100));
		cout << "Rendering finised!" << endl;
		Q_EMIT(saveImage());
	//	image.fill(283);
	//	image.save("test3.png","png");
		mutex.lock();
		image2 = image;
		mutex.unlock();
		Q_EMIT(repaint());
	}
	struct timeval t;
	gettimeofday(&t, NULL);
	cout << "time needed: " << (t.tv_sec - start_time.tv_sec) << " sec" << endl;;
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
	image.save("lightMap.png","png");
	mutex.lock();
	image2 = image;
	mutex.unlock();
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
			return graph->getColorForRay(origin, dir, recursionDepth);
			break;
		case 'r':
			//random sampling
			for(int i = 0; i < sampleCount*sampleCount; i++){
				xPix = ((float)x) + ((rand() % 1000)/1000.0);//value between 0 and 1
				yPix = ((float)y) + ((rand() % 1000)/1000.0);
				dir = myUtil::normalize(cameraMatrix * myUtil::PosHom(-abs(right) +((xPix+0.5)/((float)width)) *2*abs(right), abs(bottom)-((yPix+0.5)/((float)height))*2*abs(bottom), -near, 0));
				pixelVal = graph->getColorForRay(origin, dir, recursionDepth);
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
					pixelVal = graph->getColorForRay(origin, dir, recursionDepth);
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
					if((xPix - col.at(j)(3))*(xPix - col.at(j)(3)) + (yPix - col.at(j)(4))*(yPix - col.at(j)(4)) < (1.0/(sampleCount*sampleCount*sampleCount)))
						goto up;//sorry!
				}
				//take this sample
				dir = myUtil::normalize(cameraMatrix * myUtil::PosHom(-abs(right) +((xPix+0.5)/((float)width)) *2*abs(right), abs(bottom)-((yPix+0.5)/((float)height))*2*abs(bottom), -near, 0));
				pixelVal = graph->getColorForRay(origin, dir, recursionDepth);
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
				pixelVal = graph->getColorForRay(origin, dir, recursionDepth);
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

CVector<float> RayTracer::SampleFix(int x, int y, char kindOfSampling, int sampleCount, char kindOfReconstruction, CVector<float>& dx, CVector<float>& dy, float minDist, int p1, int p2){
	CVector<float> origin = myUtil::PosHom(0,0,0);
	origin = cameraMatrix * origin;
	vector< CVector<float> > col(sampleCount*sampleCount);
	CVector<float> dir(4,0);
	CVector<float> pixelVal;
	float strataSize;

	float xPix;
	float yPix;
	//sampling
	for(int i = 0; i < dx.size(); i++){
		xPix = ((float)x) + dx[i];
		yPix = ((float)y) + dy[i];
		dir = myUtil::normalize(cameraMatrix * myUtil::PosHom(-abs(right) +((xPix+0.5)/((float)width)) *2*abs(right), abs(bottom)-((yPix+0.5)/((float)height))*2*abs(bottom), -near, 0));
		pixelVal = graph->getColorForRay(origin, dir, recursionDepth);
		col.at(i) = myUtil::Pos5D(pixelVal(0), pixelVal(1), pixelVal(2), xPix, yPix);
	}
	return Reconstruct(col, kindOfReconstruction);
}

void RayTracer::SamplePositions(char kindOfSampling, int sampleCount, CVector<float>* dx, CVector<float>* dy, float minDist, int p1, int p2){
	float strataSize;
	vector< CVector<float> > col(sampleCount*sampleCount);

	switch (kindOfSampling) {
		case 'n':
			dx->setSize(1);
			dy->setSize(1);
			(*dx)[0] = 0.5f;
			(*dy)[0] = 0.5f;
			break;
		case 'r':
			//random sampling
			dx->setSize(sampleCount*sampleCount);
			dy->setSize(sampleCount*sampleCount);
			for(int i = 0; i < sampleCount*sampleCount; i++){
				(*dx)[i] = ((rand() % 1000)/1000.0);//value between 0 and 1
				(*dy)[i] = ((rand() % 1000)/1000.0);
			}
			break;
		case 's':
			dx->setSize(sampleCount*sampleCount);
			dy->setSize(sampleCount*sampleCount);
			//stratified sampling
			strataSize = 1.0/((float)sampleCount);
			for(int i = 0; i < sampleCount; i++){//x
				for(int j = 0; j < sampleCount; j++){//y
					(*dx)[i*sampleCount+j] = ((float)i)*strataSize + (rand() % 1000)/(1000.0*((float)sampleCount));
					(*dy)[i*sampleCount+j] = ((float)j)*strataSize + (rand() % 1000)/(1000.0*((float)sampleCount));
				}
			}
			break;
		case 'p':
			dx->setSize(sampleCount*sampleCount);
			dy->setSize(sampleCount*sampleCount);
			//poisson sampling it is cheaper
			for(int i = 0; i < sampleCount*sampleCount;){
				up:
				(*dx)[i] = ((rand() % 1000)/1000.0);//value between 0 and 1
				(*dy)[i] = ((rand() % 1000)/1000.0);
				for(int j = 0; j < i; j++){
					if(((*dx)[i] - col.at(j)(3))*((*dx)[i] - col.at(j)(3)) + ((*dy)[i] - col.at(j)(4))*((*dy)[i] - col.at(j)(4)) < (1.0/(sampleCount*sampleCount*sampleCount)))
						goto up;//sorry!
				}
				//take this sample
				col.at(i) = myUtil::Pos5D(0.0f,0.0f,0.0f,(*dx)[i], (*dy)[i]);
				i++;
			}
			break;
		case 'h':
			dx->setSize(sampleCount*sampleCount);
			dy->setSize(sampleCount*sampleCount);
			//halton sampling
			for(int i = 0; i < sampleCount*sampleCount; i++){
				(*dx)[i] = HammersleyValue(i,p1);//value between 0 and 1
				(*dy)[i] = HammersleyValue(i,p2);
			}
			cout << endl;
			break;
		default:
			cerr << "kindOfSampling: \'n\' = center of Pixel(standart)\n                \'r\' = random\n                \'s\' = stratified\n                \'p\' = poisson\n                \'h\' = halton" << endl;
			break;
	}
}

CVector<float> RayTracer::Reconstruct(vector< CVector<float> > col, char kindOfReconstruction){
	//vectorelements of col are like: [red,green,blue,xpos,ypos]
	CVector<float> color(3,0);
	CVector<float> pixValue;
	float distSum = 0;

//	for(int i = 0; i < col.size(); i++){
//		cout << col[i](3) << "\t" << col[i](4) << endl;
//	}
//	cout << endl;

	switch (kindOfReconstruction) {
		case 'b':
			//boxfilter = mean
			for(int i = 0; i < col.size(); i++){
				color += myUtil::PosHom(min(255.0f,col.at(i)(0)),min(255.0f,col.at(i)(1)),min(255.0f,col.at(i)(2)));
			}
			color *= (1.0/col.size());
			return color;
			break;
		case 'm':
			//mitchell = gauß
			for(int i = 0; i < col.size(); i++){
				pixValue = myUtil::PosHom(min(255.0f,col.at(i)(0)),min(255.0f,col.at(i)(1)),min(255.0f,col.at(i)(2)));
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
	col.clear();
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

void RayTracer::setParams(int count, char reconst, char sample, int recursion){
	rayCount = count;
	reconstruction = reconst;
	sampling = sample;
	recursionDepth = recursion;
}

QImage RayTracer::getImage(){
	return image2;
}

void RayTracer::setScene(int i){
	graph->loadScene(i);
}
