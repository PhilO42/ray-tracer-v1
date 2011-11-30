/*
 * Viewer.h
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#ifndef RAYTRACER_H_
#define RAYTRACER_H_

#include <QtGui>
#include <QPixmap>
#include <QProgressBar>
#include "RayTracer.h"

class Viewer : public QObject{
	Q_OBJECT
public:
	Viewer(QApplication* app, int argc, char *argv[]);
	virtual ~Viewer();
	void setImage(QImage image);

public Q_SIGNAL:
    void debug();
    void seeTheLightMap();

private slots:
	void draw();
	void saveImage();
	void repaint();
	void setProgress(int val);
	char getSamplingMethod();
	char getReconstructionMethod();
	int getRayCount();

private:
	QPixmap img;
	QLabel imageLabel;
	QComboBox rayCount;
	QComboBox scene;
	QComboBox recurs;
	QComboBox listReconstruction;
	QLabel recText;
	QListWidget listSampling;
	QProgressBar progress;
	RayTracer* core;
	QImage image;
	static const bool presentation = false;
	bool automation;
	QApplication* appPtr;
//	void draw();
};

#endif /* VIEWER_H_ */
