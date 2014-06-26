//===================================================================
// File:        main.cc
// Author:      Drahoslav Zan
// Email:       izan@fit.vutbr.cz
// Affiliation: Brno University of Technology,
//              Faculty of Information Technology
// Date:        Dec 2 2011
// Project:     Raytracer using CSG Scene Representation (RCSR)
//-------------------------------------------------------------------
// Copyright (C) 2011 Drahoslav Zan
//
// This file is part of RCSR.
//
// RCSR is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// RCSR is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with RCSR. If not, see <http://www.gnu.org/licenses/>.
//===================================================================
// vim: set nowrap sw=2 ts=2


#include <iostream>
#include <cstdlib>
#include <cstdio>

#include <GL/glut.h>
#include <getopt.h>
#include <omp.h>

#include "scene.h"
#include "shape.h"
#include "math.h"
#include "fps.h"
#include "materials.h"


using namespace std;


extern char *optarg;
extern int optind, opterr, optopt;

static const char * const PROG_NAME = "CSG Real-Time RayTracer";
static const int FPSCNT = 60;

static int window[2] = { 640, 480 };

static Scene *scene;

static Light *movingLight;
static Shape *movingLamp;

static FPS fps;


void onResize(int w, int h)
{
	glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, w, 0, h, -1, 1);
  glScalef(1, -1, 1);
  glTranslatef(0, -h, 0);

	int mod = w % 4;
	w = (mod) ? w - mod : w;

	window[0] = w;
	window[1] = h;

	scene->setViewPort(w, h);
}

void onDisplay(void)
{
	static char fpsText[32];

	fps.update();

	snprintf(fpsText, sizeof(fpsText), "FPS: %.2f", fps.frameRate());

	glClear(GL_COLOR_BUFFER_BIT);

	unsigned char *pixmap = scene->render();
	
	glDrawBuffer(GL_COLOR);
	glRasterPos2i(0, window[1]);

	glDrawPixels(window[0], window[1], GL_RGB, GL_UNSIGNED_BYTE, pixmap);

	glColor3f(1.0, 1.0, 0.0);
	glRasterPos2i(10, 30);
	for(const char *c = fpsText; *c != '\0'; ++c)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);

	glutSwapBuffers();
}

void onTimer(int)
{
	glutPostRedisplay();
	
	Quaternion<double> q(AngleAxis<double>(PI_4 / 20.0, Direction(0.0, 1.0, 0.0)));

	movingLight->setPosition(q * movingLight->getPosition());
	movingLamp->setPosition(q * movingLamp->getPosition());

	glutTimerFunc(1000.0 / FPSCNT, onTimer, 0);
}

void onKeyDown(unsigned char key, int, int)
{
	switch(key)
	{
	  case 27 /* ESC */ : exit(0); break;
	}
}

void moveCamera(int key, int, int)
{
	Camera &c = scene->getCamera();

	switch(key)
	{
	  case GLUT_KEY_UP:
		{
			c.setPosition(c.getPosition() + 0.1 * c.getDirection());
			break;
		}
	  case GLUT_KEY_DOWN:
		{
			c.setPosition(c.getPosition() - 0.1 * c.getDirection());
			break;
		}
	  case GLUT_KEY_LEFT:
		{
			Quaternion<double> q(AngleAxis<double>(-PI_4 / 20.0,
						Direction(0.0, 1.0, 0.0)));
			c.setPosition(q * c.getPosition());
			c.setDirection(q * c.getDirection());
			break;
		}
	  case GLUT_KEY_RIGHT:
		{
			Quaternion<double> q(AngleAxis<double>(PI_4 / 20.0,
						Direction(0.0, 1.0, 0.0)));
			c.setPosition(q * c.getPosition());
			c.setDirection(q * c.getDirection());
			break;
		}
	}
}

void addUsedMaterials()
{
	MaterialInfo info;

	info.diffuse[0] = 1.0;
	info.ambient[0] = 0.1;
	info.specular.setConstant(1.0);
	info.shininess = 10.0;

	MaterialStack::add("shiny-red", new Solid(info));

	info.emission.setZero();
	info.diffuse.setZero(); info.diffuse[1] = 1.0;
	info.ambient.setZero(); info.ambient[1] = 0.1;

	MaterialInfo info1 = info;

	MaterialStack::add("shiny-green", new Solid(info));

	info.diffuse.setZero(); info.diffuse[2] = 1.0;
	info.ambient.setZero(); info.ambient[2] = 0.1;

	MaterialStack::add("green-blue-chess", new Checkerboard(info1, info));

	MaterialStack::add("shiny-blue", new Solid(info));

	info.diffuse.setConstant(0.2);
	info.ambient.setConstant(0.0);
	info.specular.setConstant(0.3);
	info.shininess = 4.0;
	info.reflectance = 0.8;

	MaterialStack::add("mirror", new Solid(info));

	info.emission[0] = 0.6;

	info.diffuse.setConstant(0.0);
	info.ambient.setConstant(0.0);
	info.specular.setConstant(0.0);
	info.emission.setConstant(1.0); info.emission[2] = 0.5;
	info.reflectance = 0.0;

	MaterialStack::add("lamp", new Solid(info));

	info.diffuse.setConstant(0.0);
	info.ambient.setConstant(0.0);
	info.specular.setConstant(0.0);
	info.transparency = 0.7;
	info.emission.setZero();
	info.reflectance = 0.3;
	info.refraction = 1.0 / 1.488;

	MaterialStack::add("glass", new Solid(info));

	MaterialStack::add("wood", new Wood);
	MaterialStack::add("marble", new Marble);
}

int main(int argc, char **argv)
{
	// params
	int oThreads = omp_get_num_procs();
	int oDepth = 2;

	int opt;
	while((opt = getopt(argc, argv, "t:d:")) != -1)
	{
		switch(opt)
		{
			case 't':
			{
				int i = atoi(optarg);

				if(i >= 1)
					oThreads = i;

				break;
			}
			case 'd':
			{
				int i = atoi(optarg);

				if(i >= 0)
					oDepth = i;

				break;
			}
		}
	}

	omp_set_num_threads(oThreads);

	// glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(window[0], window[1]);

	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - window[0]) / 2,
		(glutGet (GLUT_SCREEN_HEIGHT) - window[1]) / 2);

	glutCreateWindow(PROG_NAME);

	glutDisplayFunc(onDisplay);
	//glutIdleFunc(onDisplay);

	glutReshapeFunc(onResize);
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(moveCamera);

	glutTimerFunc(1000.0 / FPSCNT, onTimer, 0);

	// scene
	Camera *c = new Camera(Position(0.0, 1.5, 3.0),
			Direction(0.0, -0.6, -1.0), 1.0);

	scene = new Scene(c, window[0], window[1], Color(0.5, 0.5, 1.0), oDepth);

	LightInfo linfo;

	scene->addLight(movingLight = new Light(Position(-1.3, 1.0, 0.0), linfo));

	addUsedMaterials();

	scene->addShape(movingLamp = new Sphere(Position(-1.3, 1.0, 0.0), 0.1,
				MaterialStack::get("lamp")));

	scene->addShape(new Plane(Position(0.0, 0.0, 0.0),
				Direction(0.0, 1.0, 0.0), MaterialStack::get("shiny-red")));

	scene->addShape(new Cylinder(Position(0.0, 0.0, 0.0), Direction(0.0, 1.0, 0.0),
				0.6, MaterialStack::get("mirror")));

	scene->addShape(new Sphere(Position(-1.5, 0.4, -1.5), 0.4,
				MaterialStack::get("wood")));
	scene->addShape(new Sphere(Position(-1.5, 0.4, 1.5), 0.4,
				MaterialStack::get("green-blue-chess")));
	scene->addShape(new Sphere(Position(1.5, 0.4, -1.5), 0.4,
				MaterialStack::get("marble")));
	scene->addShape(new Sphere(Position(1.5, 0.4, 1.5), 0.4,
				MaterialStack::get("glass")));

	glutMainLoop();

	return 0;
}

