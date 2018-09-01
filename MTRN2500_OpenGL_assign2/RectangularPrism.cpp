//
//  RectangularPrism.cpp
//  
//
//  Created by Ben Henderson on 1/9/18.
//

#include "RectangularPrism.hpp"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#elif defined(WIN32)
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

RectangularPrism::RectangularPrism() {
    x_length = y_length = z_length = 1;
}

RectangularPrism::RectangularPrism(double x_, double y_, double z_, double x_length_, double y_length_, double z_length_)
: Shape(x_, y_, z_)
{
    x_length = x_length_;
    y_length = y_length_;
    z_length = z_length_;
}

RectangularPrism::RectangularPrism(double x_, double y_, double z_, double x_length_, double y_length_, double z_length_, double rotation_)
: Shape(x_, y_, z_, rotation_)
{
    x_length = x_length_;
    y_length = y_length_;
    z_length = z_length_;
}

void RectangularPrism::draw() {
    glBegin(GL_LINES);
        
    glColor3f(1, 0.5f, 0.5f);
    
    glVertex3f(1, 0, 0);
    glVertex3f(1, 0, 1);
    glVertex3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    
    glVertex3f(1, 1, 0);
    glVertex3f(1, 1, 1);
    glVertex3f(0, 1, 1);
    glVertex3f(0, 1, 0);
    
    glEnd();
}
