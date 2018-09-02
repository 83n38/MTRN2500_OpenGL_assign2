//
//  TriangularPrism.hpp
//  
//
//  Created by Ben Henderson on 2/9/18.
//

#ifndef TriangularPrism_hpp
#define TriangularPrism_hpp

#include "Shape.hpp"
#include <stdio.h>
#include <cmath>

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

class TriangularPrism : public Shape {
public:
    TriangularPrism();
    TriangularPrism(float* v1_, float* v2_, float* v3_, double length_);
    TriangularPrism(float* v1_, float* v2_, float* v3_, double length_, double rotation_);
    
    void draw();
    
    void setX(double x_);
    void setY(double y_);
    void setZ(double z_);
    void setPosition(double x_, double y_, double z_);
    
protected:
    float v1[3];
    float v2[3];
    float v3[3];
    float length[3];
    float c[3];
private:
    static const int X = 0;
    static const int Y = 1;
    static const int Z = 2;
    static double findCentroidX(float* v1_, float* v2_, float* v3_, double length_);
    static double findCentroidY(float* v1_, float* v2_, float* v3_, double length_);
    static double findCentroidZ(float* v1_, float* v2_, float* v3_, double length_);
};

#endif /* TriangularPrism_hpp */
