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

class Viewer : public QObject{
	Q_OBJECT
public:
	Viewer(QApplication* app);
	virtual ~Viewer();
	void setImage(QImage image);

public Q_SIGNAL:
    void draw();
    void debug();
    void seeTheLightMap();

private slots:
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
	QListWidget listReconstruction;
	QListWidget listSampling;
	QProgressBar progress;
};

#endif /* VIEWER_H_ */
