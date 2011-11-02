/*
 * Viewer.cpp
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#include "Viewer.h"
#include "RayTracer.h"
#include <iostream>

Viewer::Viewer(QApplication* app) {
    //Widget
    QWidget window;
    window.resize(645, 485);
    window.show();
    window.setWindowTitle(QObject::tr("RayTracer v1.0"));

    //Image
    img = QPixmap(640,480);
    img.fill(QColor(255,0,0,255));
    //ImageLabel
    //imageLabel = QLabel();
    imageLabel.setGeometry(0,0,640,480);
    imageLabel.setPixmap(img);

    //Core
    RayTracer core(&img);

    //Buttons
    QPushButton button1("Save Image",0);
    QPushButton button2("Draw",0);
    QPushButton button3("Debug",0);
    QObject::connect(&button1, SIGNAL(clicked()), this, SLOT(saveImage()));
    QObject::connect(&button2, SIGNAL(clicked()), &core, SLOT(draw()));
    QObject::connect(&button3, SIGNAL(clicked()), &core, SLOT(debug()));
    //ButtonGrid
    QGridLayout buttonGrid;
    buttonGrid.addWidget(&button1,0,0);
    buttonGrid.addWidget(&button2,0,1);
    buttonGrid.addWidget(&button3,0,2);

    //MainGrid
    QGridLayout grid;
    grid.setMargin(5);
    grid.addWidget(&imageLabel,1,0);
    grid.addLayout(&buttonGrid,0,0);
    window.setLayout(&grid);

    QObject::connect(&core, SIGNAL(repaint()), this, SLOT(repaint()));

    app->exec();
}

Viewer::~Viewer() {
}


void Viewer::saveImage(){
	img.save("out/picture.png","png");
}

void Viewer::repaint(){
	imageLabel.setPixmap(img);
}
