/*
 * Viewer.cpp
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#include "Viewer.h"
#include <iostream>

using namespace std;

Viewer::Viewer(QApplication* app) {
    //Widget
    QWidget window;
    window.resize(645, 485);
    window.show();
    window.setWindowTitle(QObject::tr("RayTracer v1.0"));

    //Image
    img = QPixmap(640,480+1);
    img.load("bg.png");
    //ImageLabel
    //imageLabel = QLabel();
    imageLabel.setGeometry(0,0,640,480);
    imageLabel.setPixmap(img);

    //Core
    core = new RayTracer(&img);

    //Buttons
    QPushButton button1("Save Image",0);
    QPushButton button2("Draw Image",0);
    QPushButton button4("Draw Lightvisibility",0);
    QPushButton button3("Debug",0);
    QObject::connect(&button1, SIGNAL(clicked()), this, SLOT(saveImage()));
    QObject::connect(&button2, SIGNAL(clicked()), this, SLOT(draw()));
    QObject::connect(&button3, SIGNAL(clicked()), core, SLOT(debug()));
    QObject::connect(&button4, SIGNAL(clicked()), core, SLOT(seeTheLightMap()));
    QObject::connect(core, SIGNAL(getSamplingMethod()), this, SLOT(getSamplingMethod()));
    QObject::connect(core, SIGNAL(setProgress(int)), this, SLOT(setProgress(int)));
    QObject::connect(core, SIGNAL(getReconstructionMethod()), this, SLOT(getReconstructionMethod()));
    QObject::connect(core, SIGNAL(getRayCount()), this, SLOT(getRayCount()));
    //ButtonGrid
    QGridLayout buttonGrid;
    buttonGrid.addItem(new QSpacerItem(10,10),0,0);
    buttonGrid.addWidget(&button2);
    buttonGrid.addWidget(&button1);
    buttonGrid.addWidget(&button4);
    buttonGrid.addWidget(&button3);

    //RenderingOptions
    QLabel textSampling("Choose a sampling method:");
    listSampling.addItem("Standart Sampling");
    listSampling.addItem("Random Sampling");
    listSampling.addItem("Stratified Sampling");
    listSampling.addItem("Poisson Sampling");
    listSampling.addItem("Halton Sampling");
    listSampling.setCurrentRow(0);
    listSampling.setSelectionMode(QAbstractItemView::SingleSelection);
    buttonGrid.addItem(new QSpacerItem(10,10),5,0);
    buttonGrid.addWidget(&textSampling);
    buttonGrid.addWidget(&listSampling);

    //ReconstructionOptions
    QLabel textReconstruction("Choose a reconstruction method:");
    listReconstruction.addItem("Box Rekonstruktion");
    listReconstruction.addItem("Mitchell Rekonstruktion");
    listReconstruction.setCurrentRow(0);
    listReconstruction.setSelectionMode(QAbstractItemView::SingleSelection);
    buttonGrid.addItem(new QSpacerItem(5,5),8,0);
    buttonGrid.addWidget(&textReconstruction);
    buttonGrid.addWidget(&listReconstruction);

    //RayCountPerPixel
    QLabel textRayCount("Number of rays per pixel:");
    rayCount.addItem("1");
    rayCount.addItem("4");
    rayCount.addItem("9");
    rayCount.addItem("16");
    rayCount.addItem("25");
    buttonGrid.addItem(new QSpacerItem(5,5),11,0);
    buttonGrid.addWidget(&textRayCount);
    buttonGrid.addWidget(&rayCount);
//    progress = QProgressBar();
    progress.setRange(0,100);
    progress.setValue(0);
    buttonGrid.addWidget(&progress);
    buttonGrid.addItem(new QSpacerItem(21,20),14,0);

    //MainGrid
    QGridLayout grid;
    grid.setMargin(5);
    grid.setColumnMinimumWidth(1,200);
    grid.addWidget(&imageLabel,0,0);
    grid.addLayout(&buttonGrid,0,1);
    window.setLayout(&grid);

    QObject::connect(core, SIGNAL(repaint()), this, SLOT(repaint()));

    app->exec();
}

Viewer::~Viewer() {
	core->quit();
	delete core;
}


void Viewer::saveImage(){
	QImage image = core->getImage();
	QImage img2 = image.copy(QRect(0,0,640,480));
	img2.save("out/picture.png","png");
}

void Viewer::repaint(){
	core->mutex.lock();
	QImage image = core->getImage();
	core->mutex.unlock();
//	img.convertFromImage(image);
	imageLabel.setPixmap(img);
}

void Viewer::setProgress(int val){
	if(val == 0)
		progress.reset();
	progress.setValue(val);
}

char Viewer::getSamplingMethod(){
	switch (listSampling.currentRow()) {
		case 0:
			cout << "Standart Sampling" << endl;
			return 'n';
			break;
		case 1:
			cout << "Random Sampling" << endl;
			return 'r';
			break;
		case 2:
			cout << "Stratified Sampling" << endl;
			return 's';
			break;
		case 3:
			cout << "Poisson Sampling" << endl;
			return 'p';
			break;
		case 4:
			cout << "Halton Sampling" << endl;
			return 'h';
			break;
		default:
			cout << "Standart Sampling (sorry)" << endl;
			break;
	}
	return 'n';
}

char Viewer::getReconstructionMethod(){
	switch (listReconstruction.currentRow()) {
		case 0:
			cout << "Box Reconstruction" << endl;
			return 'b';
			break;
		case 1:
			cout << "Mitchell Reconstruction" << endl;
			return 'm';
			break;
		default:
			cout << "Box Reconstruction (sorry)" << endl;
			break;
	}
	return 'b';
}

int Viewer::getRayCount(){
	return rayCount.currentIndex()+1;
}

void Viewer::draw(){
	core->setParams(getRayCount(),getReconstructionMethod(),getSamplingMethod());
	core->start();
}
