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

public:
	RayTracer();
	RayTracer(QPixmap* _img);
	virtual ~RayTracer();
	QMutex mutex;
	void setParams(int count, char reconst, char sample);
	QImage getImage();

protected:
	void run();

private:
	std::vector<Ray> rays;
	SceneGraph* graph;
	float bottom;
	float right;
	float near;
	float far;
	static const int width = 640;
	static const int height = 480+1;
	QImage image;
	QImage image2;
	CMatrix<float> cameraMatrix;
	CVector<float> Sample(int x, int y, char kindOfSampling, int sampleCount, char kindOfReconstruction, float minDist = 0.1, int p1 = 2, int p2 = 7);
	CVector<float> Reconstruct(std::vector< CVector<float> > col, char kindOfReconstruction);
	float gauss(float dist);
	float HammersleyValue(int k, int p);
	char sampling;
	char reconstruction;
	int rayCount;
	static const int recursionDepth = 2;
};

#endif /* RAYTRACER_H_ */
