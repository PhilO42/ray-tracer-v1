/*
 * RayTracer.cpp
 *
 *  Created on: 27.10.2011
 *      Author: philipp
 */

#include "Viewer.h"

 int main(int argc, char *argv[])
 {
	 QApplication app(argc, argv);
	 Viewer tracer(&app, argc, argv);
	 return 0;
 }

