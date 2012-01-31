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
	if(sampleSize != 0){
		run2();
	}else{
		gettimeofday(&start_time, NULL);
		cout << "Rendering started!" << endl;
		cameraMatrix = graph->getCameraMatrix();

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

				//NaN-check
				if(col(0) != col(0))
					col(0) = 0;
				if(col(1) != col(1))
					col(1) = 0;
				if(col(2) != col(2))
					col(2) = 0;
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
			mutex.lock();
			image2 = image;
			mutex.unlock();
			Q_EMIT(repaint());
		}
		struct timeval t;
		gettimeofday(&t, NULL);
		cout << "time needed: " << (t.tv_sec - start_time.tv_sec) << " sec" << endl;
	}

	//((BVH*)graph->objects[0])->printCount();
}

void RayTracer::run2(){
	gettimeofday(&start_time, NULL);
	cout << "Rendering started!" << endl;
	cameraMatrix = graph->getCameraMatrix();
	cout << "Drawing image with " << sampling << " " << reconstruction << " and " << rayCount*rayCount << " rays per pixel" << endl;

	colors = new std::vector< CVector<float> >* [width];
	for (int x = 0; x < width; x++) {
		colors[x] = new std::vector< CVector<float> > [height];
		for (int y = 0; y < height; y++) {
			colors[x][y] = std::vector< CVector<float> >();
		}
	}

	#pragma omp parallel for schedule(dynamic, CHUNKSIZE)
	for(int y = 0; y < height-1; y++){
		if(y%(height/10) == 0){
			#pragma omp critical
			{
				cout << " " << (int)(y/(height/10))*10 << "% finished" << endl;
				Q_EMIT(setProgress((int)(y/(height/10))*10));
			}
		}
		for(int x = 0; x < width; x++){
			colors[x][y] = SamplePur(x, y, sampling, rayCount);
		}
	}

	#pragma omp critical
		image = Reconstruct2(reconstruction, sampleSize);

	#pragma omp critical
	{
		cout << "100% finished" << endl;
		Q_EMIT(setProgress(100));
		cout << "Rendering finised!" << endl;
		Q_EMIT(saveImage());
		mutex.lock();
		image2 = image;
		mutex.unlock();
		Q_EMIT(repaint());
	}
	struct timeval t;
	gettimeofday(&t, NULL);
	cout << "time needed: " << (t.tv_sec - start_time.tv_sec) << " sec" << endl;;

	if (colors != NULL) {
		for (int i = 0; i < width; i++) {
			delete[] colors[i];
		}
		delete[] colors;
	}
}

void RayTracer::seeTheLightMap(){
	CMatrix<float> cameraMatrix = graph->getCameraMatrix();
	CVector<float> origin = myUtil::PosHom(0,0,0);
	origin = cameraMatrix * origin;
	for(int y = 0; y < height-1; y++){
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
				col.at(i) = myUtil::Pos5D(pixelVal(0), pixelVal(1), pixelVal(2), xPix-x, yPix-y);
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
					col.at(i*sampleCount+j) = myUtil::Pos5D(pixelVal(0), pixelVal(1), pixelVal(2), xPix-x, yPix-y);
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
				col.at(i) = myUtil::Pos5D(pixelVal(0), pixelVal(1), pixelVal(2), xPix-x, yPix-y);
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
				col.at(i) = myUtil::Pos5D(pixelVal(0), pixelVal(1), pixelVal(2), xPix-x, yPix-y);
			}
			return Reconstruct(col, kindOfReconstruction);
			break;
		default:
			cerr << "kindOfSampling: \'n\' = center of Pixel(standart)\n                \'r\' = random\n                \'s\' = stratified\n                \'p\' = poisson\n                \'h\' = halton" << endl;
			break;
	}
	return CVector<float>(3,0);
}

std::vector< CVector<float> > RayTracer::SamplePur(int x, int y, char kindOfSampling, int sampleCount, float minDist, int p1, int p2){
	CVector<float> origin = myUtil::PosHom(0,0,0);
	origin = cameraMatrix * origin;
	CVector<float> dir(4,0);
	CVector<float> pixelVal;
	vector< CVector<float> > back;
	float strataSize;

	float xPix;
	float yPix;

	switch (kindOfSampling) {
		case 'n':
			//standart sampling one ray per center of pixel
			xPix = ((float)x) + 0.5;
			yPix = ((float)y) + 0.5;
			dir = myUtil::normalize(cameraMatrix * myUtil::PosHom(-abs(right) +((xPix+0.5)/((float)width)) *2*abs(right), abs(bottom)-((yPix+0.5)/((float)height))*2*abs(bottom), -near, 0));
			pixelVal = graph->getColorForRay(origin, dir, recursionDepth);
			back.push_back(myUtil::Pos5D(pixelVal(0), pixelVal(1), pixelVal(2), xPix, yPix));
			return back;
			break;
		case 'r':
			//random sampling
			for(int i = 0; i < sampleCount*sampleCount; i++){
				xPix = ((float)x) + ((rand() % 1000)/1000.0);//value between 0 and 1
				yPix = ((float)y) + ((rand() % 1000)/1000.0);
				dir = myUtil::normalize(cameraMatrix * myUtil::PosHom(-abs(right) +((xPix+0.5)/((float)width)) *2*abs(right), abs(bottom)-((yPix+0.5)/((float)height))*2*abs(bottom), -near, 0));
				pixelVal = graph->getColorForRay(origin, dir, recursionDepth);
				back.push_back(myUtil::Pos5D(pixelVal(0), pixelVal(1), pixelVal(2), xPix, yPix));
			}
			return back;
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
					back.push_back(myUtil::Pos5D(pixelVal(0), pixelVal(1), pixelVal(2), xPix, yPix));
				}
			}
			return back;
			break;
		case 'p':
			//poisson sampling it is cheaper
			for(int i = 0; i < sampleCount*sampleCount;){
				up:
				xPix = ((float)x) + ((rand() % 1000)/1000.0);//value between 0 and 1
				yPix = ((float)y) + ((rand() % 1000)/1000.0);
				for(int j = 0; j < i; j++){
					if((xPix - back.at(j)(3))*(xPix - back.at(j)(3)) + (yPix - back.at(j)(4))*(yPix - back.at(j)(4)) < (1.0/(sampleCount*sampleCount*sampleCount)))
						goto up;//sorry!
				}
				//take this sample
				dir = myUtil::normalize(cameraMatrix * myUtil::PosHom(-abs(right) +((xPix+0.5)/((float)width)) *2*abs(right), abs(bottom)-((yPix+0.5)/((float)height))*2*abs(bottom), -near, 0));
				pixelVal = graph->getColorForRay(origin, dir, recursionDepth);
				back.push_back(myUtil::Pos5D(pixelVal(0), pixelVal(1), pixelVal(2), xPix, yPix));
				i++;
			}
			return back;
			break;
		case 'h':
			//halton sampling
			for(int i = 0; i < sampleCount*sampleCount; i++){
				xPix = ((float)x) + HammersleyValue(i,p1);//value between 0 and 1
				yPix = ((float)y) + HammersleyValue(i,p2);
				dir = myUtil::normalize(cameraMatrix * myUtil::PosHom(-abs(right) +((xPix+0.5)/((float)width)) *2*abs(right), abs(bottom)-((yPix+0.5)/((float)height))*2*abs(bottom), -near, 0));
				pixelVal = graph->getColorForRay(origin, dir, recursionDepth);
				back.push_back(myUtil::Pos5D(pixelVal(0), pixelVal(1), pixelVal(2), xPix, yPix));
			}
			return back;
			break;
		default:
			cerr << "kindOfSampling: \'n\' = center of Pixel(standart)\n                \'r\' = random\n                \'s\' = stratified\n                \'p\' = poisson\n                \'h\' = halton" << endl;
			break;
	}
	return back;
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

QImage RayTracer::Reconstruct2(char kindOfReconstruction, int sampleSize){
	//vectorelements of col are like: [red,green,blue,xpos,ypos]

	#pragma omp parallel for schedule(dynamic, CHUNKSIZE)

	for(int y = 0; y < height-1; y++){
		for(int x = 0; x < width; x++){
			std::vector< CVector<float> > col = getRaysAt(x,y, sampleSize);
			CVector<float> color(3,0);
			CVector<float> pixValue;
			float distSum = 0;
			QColor qcolor;
//			if(col.size() != 9)
//				cout << col.size() << " " << x << " " << y << " " << endl;
			switch (kindOfReconstruction) {
				case 'b':
					//boxfilter = mean
					for(int i = 0; i < col.size(); i++){
						color += myUtil::PosHom(min(255.0f,col.at(i)(0)),min(255.0f,col.at(i)(1)),min(255.0f,col.at(i)(2)));
					}
					if(col.size() != 0)
						color *= (1.0/col.size());
					qcolor = QColor(min((int)color(0),255),min((int)color(1),255),min((int)color(2),255),255);

					//#pragma omp critical
					mutex.lock();
					image.setPixel(x,y,qcolor.rgb());
					mutex.unlock();

					break;
				case 'm':
					//mitchell = gauß
					for(int i = 0; i < col.size(); i++){
						pixValue = myUtil::PosHom(min(255.0f,col.at(i)(0)),min(255.0f,col.at(i)(1)),min(255.0f,col.at(i)(2)));
						int px = sqrt((((float)x) - col.at(i)(3))*(((float)x) - col.at(i)(3)));
						int py = sqrt((((float)y) - col.at(i)(4))*(((float)y) - col.at(i)(4)));
						float xDist = px - 0.5;//signed
						float yDist = py - 0.5;
						float weight = gauss(sqrt(xDist*xDist + yDist*yDist),0.6);//Gewichtung mit Abstand
						pixValue *= weight;
						distSum += weight;
						color +=  pixValue;
					}
					if(distSum != 0)
						color *= (1.0/distSum);//Normalisierung
					qcolor = QColor(min((int)color(0),255),min((int)color(1),255),min((int)color(2),255),255);

					//#pragma omp critical
					mutex.lock();
					image.setPixel(x,y,qcolor.rgb());
					mutex.unlock();

					break;
				default:
					cerr << "kindOfReconstruction: \'b\' = box(standart)\n                      \'m\' = mitchell" << endl;
					break;
			}
			col.clear();
		}
	}
	return image;
}

std::vector< CVector<float> > RayTracer::getRaysAt(int x, int y, int sampleSize){
	std::vector< CVector<float> > col;
	for(int xVal = x-sampleSize; xVal <= x+sampleSize; xVal++){
		for(int yVal = y-sampleSize; yVal <= y+sampleSize; yVal++){
			if(xVal >= 0 && xVal < width-1 && yVal >= 0 && yVal < height){
				for(int i = 0; i < colors[xVal][yVal].size(); i++){
					CVector<float> pixVal = colors[xVal][yVal][i];
					if(maxDist(pixVal[3],(float)x+0.5,pixVal[4],(float)y+0.5) <= sampleSize)
						col.push_back(colors[xVal][yVal][i]);
				}
			}
		}
	}
	return col;
}

float RayTracer::maxDist(float x1, float x2, float y1, float y2){
	return max(sqrt((x1-x2)*(x1-x2)),sqrt((y1-y2)*(y1-y2)));
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
				float xDist = col.at(i)(3)-0.5;//signed
				float yDist = col.at(i)(4)-0.5;
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

float RayTracer::gauss(float dist, float sigma){
	return (1.0/(sqrt(2.0*3.14159)*sigma))*exp(-0.5*(dist/sigma)*(dist/sigma));
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
