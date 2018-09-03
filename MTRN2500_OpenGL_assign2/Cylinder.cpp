//
//  Cyinder.cpp
//  
//
//  Created by Ben Henderson on 2/9/18.
//

#include "Cylinder.hpp"
#include <cmath>
#include <iostream>
#include <cstdio>

Cylinder::Cylinder() {
    x = y = z = 0;
    length = 1;
    radius = 1;
}

Cylinder::Cylinder(double x_, double y_, double z_, double radius_, double length_)
: Shape(x_, y_, z_)
{
    radius = radius_;
    length = length_;
}

Cylinder::Cylinder(double x_, double y_, double z_, double radius_, double length_, double rotation_)
: Shape(x_, y_, z_, rotation_)
{
    radius = radius_;
    length = length_;
}

void Cylinder::draw() {
    
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    
    glPushMatrix();
    
    // positions the cube correctly
    this->positionInGL();
    glTranslatef(0, 0, -length/2);
    
    // colours correctly
    this->setColorInGL();
    
    gluCylinder(quadratic, radius, radius, length, 32, 32);
    
    
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, 0);
    for (int i = 0; i <= 32; i++) {
        glVertex3f(radius*sinf(2*pi*i/32.f), radius*cosf(2*pi*i/32.f), 0);
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, length);
    for (int i = 0; i <= 32; i++) {
        glVertex3f(radius*sin(2*pi*i/32), radius*cos(2*pi*i/32), length);
    }
    glEnd();
    
    // black edges
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i <= 32; i++) {
        glVertex3f(radius*sin(2*pi*i/32), radius*cos(2*pi*i/32), 0);
    }
    glEnd();
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i <= 32; i++) {
        glVertex3f(radius*sin(2*pi*i/32), radius*cos(2*pi*i/32), length);
    }
    glEnd();
    glPopMatrix();
    
}