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

class RayTracer : public QObject{
	Q_OBJECT
public slots:
	void debug();
	void draw();
	void seeTheLightMap();
public:
	Q_SIGNALS:
	void repaint();

public:
	RayTracer(QPixmap* _img);
	virtual ~RayTracer();

private:
	QPixmap* img;
	std::vector<Ray> rays;
	SceneGraph* graph;
	float bottom;
	float right;
	float near;
	float far;
	static const int width = 640;
	static const int height = 480;
	QImage image;
};

#endif /* RAYTRACER_H_ */
