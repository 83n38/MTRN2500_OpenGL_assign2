//
//  Cyinder.hpp
//  
//
//  Created by Ben Henderson on 2/9/18.
//

#ifndef Cyinder_hpp
#define Cyinder_hpp

#include "Shape.hpp"
#include <stdio.h>

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

class Cylinder : public Shape {
public:
    Cylinder();
    Cylinder(double x_, double y_, double z_, double radius_, double length_);
    Cylinder(double x_, double y_, double z_, double radius_, double length_, double rotation_);
    void draw();
protected:
    double radius;
    double length;
private:
    float pi = 3.1415;

};

#endif /* Cyinder_hpp */
