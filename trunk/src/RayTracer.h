/*
 * RayTracer.h
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#ifndef TRACER_H_
#define TRACER_H_

#include <QtGui>
#include "Ray.h"
#include "SceneGraph.h"
#include "mathe/CVector.h"

class RayTracer : public QThread{
	Q_OBJECT
public slots:
	void debug();
	//void draw();
	void seeTheLightMap();

public:
	Q_SIGNALS:
	void repaint();
	void setProgress(int val);
	char getSamplingMethod();
	char getReconstructionMethod();
	int getRayCount();
	void saveImage();

public:
	RayTracer();
	RayTracer(QPixmap* _img, bool pres, int _width, int _height, int argc, char *argv[]);
	virtual ~RayTracer();
	QMutex mutex;
	QMutex mutexImg;
	void setParams(int count, char reconst, char sample, int recursion);
	QImage getImage();
	void setScene(int i);

protected:
	void run();
	void run2();

private:
	std::vector<Ray> rays;
	SceneGraph* graph;
	float bottom;
	float right;
	float near;
	float far;
	int width;//640
	int height;//480
	QImage image;
	QImage image2;
	CMatrix<float> cameraMatrix;
	CVector<float> Sample(int x, int y, char kindOfSampling, int sampleCount, char kindOfReconstruction, float minDist = 0.1, int p1 = 2, int p2 = 7);
	std::vector< CVector<float> > SamplePur(int x, int y, char kindOfSampling, int sampleCount, float minDist = 0.1, int p1 = 2, int p2 = 7);
	CVector<float> SampleFix(int x, int y, char kindOfSampling, int sampleCount, char kindOfReconstruction, CVector<float>& dx, CVector<float>& dy, float minDist = 0.1, int p1 = 2, int p2 = 7);
	void SamplePositions(char kindOfSampling, int sampleCount, CVector<float>* dx, CVector<float>* dy, float minDist = 0.1, int p1 = 2, int p2 = 7);
	CVector<float> Reconstruct(std::vector< CVector<float> > col, char kindOfReconstruction);
	QImage Reconstruct2(char kindOfReconstruction, int sampleSize);
	std::vector< CVector<float> > getRaysAt(int x, int y, int sampleSize);
	float maxDist(float x1, float x2, float y1, float y2);
	float gauss(float dist, float sigma = 1.0);
	float HammersleyValue(int k, int p);
	char sampling;
	char reconstruction;
	int rayCount;
	int recursionDepth;
	bool presentation;
	static const int sampleSize = 1;
	std::vector< CVector<float> >** colors;
};

#endif /* RAYTRACER_H_ */
