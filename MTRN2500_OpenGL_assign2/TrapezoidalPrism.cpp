//
//  TrapezoidalPrism.cpp
//  
//
//  Created by Ben Henderson on 2/9/18.
//

#include "TrapezoidalPrism.hpp"

#include "VectorMaths.hpp"

#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <cmath>


TrapezoidalPrism::TrapezoidalPrism() {
    v1[X] = 1.f; v1[Y] = 0.f; v1[Z] = 0.f;
    v2[X] = 0.5f; v2[Y] = 0.f; v2[Z] = 1.f;
    v3[X] = -0.5f; v2[Y] = 0.f; v3[Z] = 1.f;
    v4[X] = -1.f; v4[Y] = 0.f; v4[Z] = 0.f;
    length[X] = 0.f; length[Y] = -1.f; length[Z] = 0.f;
    c[X] = x = this->findCentroidX(v1, v2, v3, v4, 1.0);
    //std::cout << x << std::endl;
    c[Y] = y = this->findCentroidY(v1, v2, v3, v4, 1.0);
    //std::cout << "Y centroid: " << y << std::endl;
    c[Z] = z = this->findCentroidZ(v1, v2, v3, v4, 1.0);
    //std::cout << "Z centroid: "<< z << std::endl;
}

void TrapezoidalPrism::init(float* v1_, float* v2_, float* v3_, float* v4_, double length_) {
    std::copy(v1_, v1_+3, v1);
    std::copy(v2_, v2_+3, v2);
    std::copy(v3_, v3_+3, v3);
    std::copy(v4_, v4_+3, v4);
    // need to figure out the plane parrallel to all these points
    // then find unit vector normal to this plane
    // and the length vector will be a scalar multiple of this normal vector
    float* v12 = scos::VectorMaths::minus(v2, v1);
    float* v13 = scos::VectorMaths::minus(v3, v1);
    float* v14 = scos::VectorMaths::minus(v4, v1);
    float* normal = scos::VectorMaths::cross(v12, v13);
    scos::VectorMaths::normalise(normal);
    
    // make sure vertice 4 is on the same plane, so
    // v14new = proj(v14 onto normal) = v14 - ((v14.n)/n^2) x n
    normal[X] *= scos::VectorMaths::dot(v14, normal)/(pow(normal[X], 2) + pow(normal[Y],2) + pow(normal[Z],2));
    normal[Y] *= scos::VectorMaths::dot(v14, normal)/(pow(normal[X], 2) + pow(normal[Y],2) + pow(normal[Z],2));
    normal[Z] *= scos::VectorMaths::dot(v14, normal)/(pow(normal[X], 2) + pow(normal[Y],2) + pow(normal[Z],2));
    // so now corrected v4 = v14new + v1
    float* v14new = scos::VectorMaths::plus(scos::VectorMaths::minus(v14, normal), v1);
    std::copy(v14new, v14new+3, v4);
    std::cout << "X: " << v4[X] << "Y :" << v4[Y] << "Z: " << v4[Z] << std::endl;
    
    // multiple of length in the normal direction
    length[X] = float(length_*normal[X]); length[Y] = float(length_*normal[Y]); length[Z] = float(length_*normal[Z]);
    c[X] = x; c[Y] = y; c[Z] = z;
}

TrapezoidalPrism::TrapezoidalPrism(float* v1_, float* v2_, float* v3_, float* v4_, double length_)
: Shape(findCentroidX(v1_, v2_, v3_, v4_, length_), findCentroidY(v1_, v2_, v3_, v4_, length_), findCentroidZ(v1_, v2_, v3_, v4_, length_))
{
    init(v1_, v2_, v3_, v4_, length_);
}

TrapezoidalPrism::TrapezoidalPrism(float* v1_, float* v2_, float* v3_, float* v4_, double length_, double rotation_)
: Shape(findCentroidX(v1_, v2_, v3_, v4_, length_), findCentroidY(v1_, v2_, v3_, v4_, length_), findCentroidZ(v1_, v2_, v3_, v4_, length_), rotation_)
{
    init(v1_, v2_, v3_, v4_, length_);
}
void TrapezoidalPrism::draw() {
    
    glPushMatrix();
    
    // positions the cube correctly
    this->positionInGL();
    
    // colours correctly
    this->setColorInGL();
    
    float* vc1 = scos::VectorMaths::minus(v1, c);
    float* vc2 = scos::VectorMaths::minus(v2, c);
    float* vc3 = scos::VectorMaths::minus(v3, c);
    float* vc4 = scos::VectorMaths::minus(v4, c);
    
    glBegin(GL_QUADS);
    glVertex3f(vc1[X], vc1[Y], vc1[Z]);
    glVertex3f(vc2[X], vc2[Y], vc2[Z]);
    glVertex3f(vc3[X], vc3[Y], vc3[Z]);
    glVertex3f(vc4[X], vc4[Y], vc4[Z]);
    glEnd();
    
    float* f1 = scos::VectorMaths::minus(vc1, length);
    float* f2 = scos::VectorMaths::minus(vc2, length);
    float* f3 = scos::VectorMaths::minus(vc3, length);
    float* f4 = scos::VectorMaths::minus(vc4, length);
    
    glBegin(GL_QUADS);
    glVertex3f(f1[X], f1[Y], f1[Z]);
    glVertex3f(f2[X], f2[Y], f2[Z]);
    glVertex3f(f3[X], f3[Y], f3[Z]);
    glVertex3f(f4[X], f4[Y], f4[Z]);
    glEnd();
    
    glBegin(GL_QUAD_STRIP);
    glVertex3f(vc1[X], vc1[Y], vc1[Z]);
    glVertex3f(f1[X], f1[Y], f1[Z]);
    glVertex3f(vc2[X], vc2[Y], vc2[Z]);
    glVertex3f(f2[X], f2[Y], f2[Z]);
    glVertex3f(vc3[X], vc3[Y], vc3[Z]);
    glVertex3f(f3[X], f3[Y], f3[Z]);
    glVertex3f(vc4[X], vc4[Y], vc4[Z]);
    glVertex3f(f4[X], f4[Y], f4[Z]);
    glVertex3f(vc1[X], vc1[Y], vc1[Z]);
    glVertex3f(f1[X], f1[Y], f1[Z]);
    glEnd();

    // draw edges in black
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3f(vc1[X], vc1[Y], vc1[Z]);
    glVertex3f(vc2[X], vc2[Y], vc2[Z]);
    glVertex3f(vc3[X], vc3[Y], vc3[Z]);
    glVertex3f(vc4[X], vc4[Y], vc4[Z]);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3f(f1[X], f1[Y], f1[Z]);
    glVertex3f(f2[X], f2[Y], f2[Z]);
    glVertex3f(f3[X], f3[Y], f3[Z]);
    glVertex3f(f4[X], f4[Y], f4[Z]);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(vc1[X], vc1[Y], vc1[Z]);
    glVertex3f(f1[X], f1[Y], f1[Z]);
    glVertex3f(vc2[X], vc2[Y], vc2[Z]);
    glVertex3f(f2[X], f2[Y], f2[Z]);
    glVertex3f(vc3[X], vc3[Y], vc3[Z]);
    glVertex3f(f3[X], f3[Y], f3[Z]);
    glVertex3f(vc4[X], vc4[Y], vc4[Z]);
    glVertex3f(f4[X], f4[Y], f4[Z]);
    glEnd();
    glPopMatrix();
}

void TrapezoidalPrism::setX(double x_) {
    Shape::setX(x_);
    v1[X] += x_ - c[X];
    v2[X] += x_ - c[X];
    v3[X] += x_ - c[X];
    v4[X] += x_ - c[X];
    c[X] = x_;
};

void TrapezoidalPrism::setY(double y_) {
    Shape::setY(y_);
    v1[Y] += y_ - c[Y];
    v2[Y] += y_ - c[Y];
    v3[Y] += y_ - c[Y];
    v4[Y] += y_ - c[Y];
    c[Y] = y_;
};

void TrapezoidalPrism::setZ(double z_) {
    Shape::setZ(z_);
    v1[Z] += z_ - c[Z];
    v2[Z] += z_ - c[Z];
    v3[Z] += z_ - c[Z];
    v4[Z] += z_ - c[Z];
    c[Z] = z_;
};

void TrapezoidalPrism::setPosition(double x_, double y_, double z_) {
    this->setX(x_);
    this->setY(y_);
    this->setZ(z_);
};

float* TrapezoidalPrism::findCentroid(float *v1_, float *v2_, float *v3_, float* v4_, double length_) {
    // correct v4 if its not on the plane of the other vertices
    float* v12 = scos::VectorMaths::minus(v2_, v1_);
    float* v13 = scos::VectorMaths::minus(v3_, v1_);
    float* v14 = scos::VectorMaths::minus(v4_, v1_);
    float* normal = scos::VectorMaths::cross(v12, v13);
    scos::VectorMaths::normalise(normal);
    
    // make sure vertice 4 is on the same plane, so
    // v14new = proj(v14 onto normal) = v14 - ((v14.n)/n^2) x n
    float normal_proj[3];
    normal_proj[X] *= scos::VectorMaths::dot(v14, normal)/(pow(normal[X], 2) + pow(normal[Y],2) + pow(normal[Z],2));
    normal_proj[Y] *= scos::VectorMaths::dot(v14, normal)/(pow(normal[X], 2) + pow(normal[Y],2) + pow(normal[Z],2));
    normal_proj[Z] *= scos::VectorMaths::dot(v14, normal)/(pow(normal[X], 2) + pow(normal[Y],2) + pow(normal[Z],2));
    // so now corrected v4 = v14new + v1
    float* v14new = scos::VectorMaths::plus(scos::VectorMaths::minus(v14, normal_proj), v1_);
    std::copy(v14new, v14new+3, v4_);
    
    
    float c[3] = {(v1_[X]+v2_[X]+v3_[X]+v4_[X])/4, (v1_[Y]+v2_[Y]+v3_[Y]+v4_[Y])/4, (v1_[Z]+v2_[Z]+v3_[Z]+v4_[Z])/4};
    
    // multiple of length in the normal direction
    float length[3];
    // each term is divided by two because we ant the middle of the prism
    length[X] = float(length_*normal[X]/2); length[Y] = float(length_*normal[Y]/2); length[Z] = float(length_*normal[Z]/2);
    float* centroid = scos::VectorMaths::minus(c, length);
    return centroid;
}

double TrapezoidalPrism::findCentroidX(float *v1_, float *v2_, float *v3_, float* v4_, double length_) {
    return findCentroid(v1_, v2_, v3_, v4_, length_)[X];
}

double TrapezoidalPrism::findCentroidY(float *v1_, float *v2_, float *v3_, float* v4_, double length_) {
    return findCentroid(v1_, v2_, v3_, v4_, length_)[Y];
}

double TrapezoidalPrism::findCentroidZ(float *v1_, float *v2_, float *v3_, float* v4_, double length_) {
    return findCentroid(v1_, v2_, v3_, v4_, length_)[Z];
}

