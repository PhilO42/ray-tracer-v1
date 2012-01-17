/*
 * Viewer.cpp
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#include "Viewer.h"
#include <iostream>

using namespace std;

Viewer::Viewer(QApplication* app, int argc, char *argv[]) {
	appPtr = app;
    //Widget
    QWidget window;
    window.resize(width+5, height+5);
    window.show();
    window.setWindowTitle(QObject::tr("RayTracer v1.0"));

    automation = false;
    if(argc > 2){
    	if(string(argv[2]) == "-a"){
    		automation = true;
    		cout << "automating" << endl;
    	}
    }

    //Image
    img = QPixmap(width,height+1);
    img.fill((uint)0);
    if(!presentation){
        img.load("bg.png");
    }
    //ImageLabel
    //imageLabel = QLabel();
    imageLabel.setGeometry(0,0,width,height);
    imageLabel.setPixmap(img);

    //Core
    core = new RayTracer(&img, presentation, width, height, argc, argv);

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
    QObject::connect(core, SIGNAL(saveImage()), this, SLOT(saveImage()));
    QObject::connect(core, SIGNAL(getReconstructionMethod()), this, SLOT(getReconstructionMethod()));
    QObject::connect(core, SIGNAL(getRayCount()), this, SLOT(getRayCount()));
    //ButtonGrid
    QGridLayout buttonGrid;
    //buttonGrid.addItem(new QSpacerItem(10,10),0,0);
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
    if(automation)
    	listSampling.setCurrentRow(3);//3
    QLabel textScene("Choose scene:");
    scene.addItem("4 spheres");
    scene.addItem("axis");
    buttonGrid.addWidget(&textScene);
    buttonGrid.addWidget(&scene);
    scene.addItem("table scene");
	scene.addItem("table scene with duck");
	scene.addItem("ground only");
	scene.addItem("empty");
	scene.addItem("1 Sphere");
	scene.addItem("2 Spheres");
	scene.addItem("4 Spheres");
	scene.addItem("new Spheres");
	scene.addItem("Pool");
	if (automation) {
		scene.setCurrentIndex(3);
	} else {
		scene.setCurrentIndex(2);
	}
	if(presentation)
		scene.setCurrentIndex(0);
	buttonGrid.addWidget(&textSampling);
	buttonGrid.addWidget(&listSampling);

    //ReconstructionOptions
    QLabel textReconstruction("Choose a reconstruction method:");
    listReconstruction.addItem("Box Rekonstruktion");
    listReconstruction.addItem("Mitchell Rekonstruktion");
    if(automation)
    	listReconstruction.setCurrentIndex(1);
    //buttonGrid.addItem(new QSpacerItem(5,5),8,0);
    buttonGrid.addWidget(&textReconstruction);
    buttonGrid.addWidget(&listReconstruction);

    //RayCountPerPixel
    QLabel textRayCount("Number of rays per pixel:");
    rayCount.addItem("1");
    rayCount.addItem("4");
    rayCount.addItem("9");
    rayCount.addItem("16");
    rayCount.addItem("100");
    rayCount.addItem("2500");
    rayCount.addItem("10000");
    //buttonGrid.addItem(new QSpacerItem(5,5),14,0);
    if(automation)
    	rayCount.setCurrentIndex(2);
    buttonGrid.addWidget(&textRayCount);
    buttonGrid.addWidget(&rayCount);
	recurs.addItem("1");
	recurs.addItem("2");
	recurs.addItem("3");
	recurs.addItem("4");
	recurs.addItem("5");
	recurs.addItem("6");
	if(automation)
		recurs.setCurrentIndex(5);//5
	if(!presentation){
		buttonGrid.addWidget(new QLabel("Recursion Depth:"));
		buttonGrid.addWidget(&recurs);
	}
    progress.setRange(0,100);
    progress.setValue(0);
    buttonGrid.addWidget(&progress);

    //MainGrid
    QGridLayout grid;
    grid.setMargin(5);
    grid.setColumnMinimumWidth(1,200);
    grid.addWidget(&imageLabel,0,0);
    grid.addLayout(&buttonGrid,0,1);
    window.setLayout(&grid);

    QObject::connect(core, SIGNAL(repaint()), this, SLOT(repaint()));

	if(automation){
		Q_EMIT(draw());
	}
    app->exec();
}

Viewer::~Viewer() {
	core->quit();
	delete core;
}

bool file_exists(const char *filename)
{
  ifstream ifile(filename);
  return ifile;
}

void Viewer::saveImage(){
	QImage image = core->getImage();
	QImage img2 = image.copy(QRect(0,0,width,height));
	int i = 0;
	while(true){
		stringstream str;
		if(presentation){
			str << "out2/picture_" << i << ".png";
		}else{
			str << "out/picture_" << i << ".png";
		}
		if(!file_exists(str.str().c_str())){
			break;
		}
		i++;
	}

	stringstream str;
	if(presentation){
		str << "out2/picture_" << i << ".png";
	}else{
		str << "out/picture_" << i << ".png";
	}
	cout << str.str() << endl;
	img2.save(str.str().c_str(),"png");
	if(automation){
		sleep(10);
		core->quit();
		appPtr->exit(0);
	}
}

void Viewer::repaint(){
	core->mutex.lock();
	image = core->getImage();
	core->mutex.unlock();
	img = img.fromImage(image);
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
			cout << "Default Sampling (sorry)" << endl;
			break;
	}
	return 'n';
}

char Viewer::getReconstructionMethod(){
	switch (listReconstruction.currentIndex()) {
		case 0:
			cout << "Box Reconstruction" << endl;
			return 'b';
			break;
		case 1:
			cout << "Mitchell Reconstruction" << endl;
			return 'm';
			break;
		default:
			cout << "Default Reconstruction (sorry)" << endl;
			break;
	}
	return 'b';
}

int Viewer::getRayCount() {
	switch (rayCount.currentIndex()) {
	case 0:
		return 1;
		break;
	case 1:
		return 2;
		break;
	case 2:
		return 3;
		break;
	case 3:
		return 4;
		break;
	case 4:
		return 10;
		break;
	case 5:
		return 50;
		break;
	case 6:
		return 100;
		break;
	default:
		return 1;
		break;
	}
}

void Viewer::draw(){
	if(!presentation){
		core->setParams(getRayCount(),getReconstructionMethod(),getSamplingMethod(),recurs.currentIndex()+1);
	}else{
		core->setParams(getRayCount(),getReconstructionMethod(),getSamplingMethod(),1);
	}
	core->setScene(scene.currentIndex());
	core->start();
}
