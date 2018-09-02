//
//  TriangularPrism.cpp
//  
//
//  Created by Ben Henderson on 2/9/18.
//

#include "TriangularPrism.hpp"
#include "VectorMaths.hpp"

#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <iterator>


TriangularPrism::TriangularPrism() {
    v1[X] = 1.f; v1[Y] = 0.f; v1[Z] = 0.f;
    v2[X] = -0.5f; v2[Y] = 0.f; v2[Z] = float(sqrt(3)/2);
    v3[X] = -0.5f; v2[Y] = 0.f; v2[Z] = float(-sqrt(3)/2);
    length[X] = 0.f; length[Y] = 1.f; length[Z] = 0.f;
    c[X] = x; c[Y] = y; c[Z] = z;
}

TriangularPrism::TriangularPrism(float* v1_, float* v2_, float* v3_, double length_)
: Shape(findCentroidX(v1_, v2_, v3_, length_), findCentroidY(v1_, v2_, v3_, length_), findCentroidZ(v1_, v2_, v3_, length_))
{
    std::copy(v1_, v1_+3, v1);
    std::copy(v2_, v2_+3, v2);
    std::copy(v3_, v3_+3, v3);
    // need to figure out the plane parrallel to all these points
    // then find unit vector normal to this plane
    // and the length vector will be a scalar multiple of this normal vector
    float* v12 = scos::VectorMaths::minus(v2, v1);
    float* v13 = scos::VectorMaths::minus(v3, v1);
    float* normal = scos::VectorMaths::cross(v12, v13);
    scos::VectorMaths::normalise(normal);
    // multiple of length in the normal direction
    length[X] = float(length_*normal[X]); length[Y] = float(length_*normal[Y]); length[Z] = float(length_*normal[Z]);
    c[X] = x; c[Y] = y; c[Z] = z;
}

TriangularPrism::TriangularPrism(float* v1_, float* v2_, float* v3_, double length_, double rotation_)
: Shape(findCentroidX(v1_, v2_, v3_, length_), findCentroidY(v1_, v2_, v3_, length_), findCentroidZ(v1_, v2_, v3_, length_), rotation_)
{
    std::copy(v1_, v1_+3, v1);
    std::copy(v2_, v2_+3, v2);
    std::copy(v3_, v3_+3, v3);
    // need to figure out the plane parrallel to all these points
    // then find unit vector normal to this plane
    // and the length vector will be a scalar multiple of this normal vector
    float* v12 = scos::VectorMaths::minus(v2, v1);
    float* v13 = scos::VectorMaths::minus(v3, v1);
    float* normal = scos::VectorMaths::cross(v12, v13);
    scos::VectorMaths::normalise(normal);
    // multiple of length in the normal direction
    length[X] = float(length_*normal[X]); length[Y] = float(length_*normal[Y]); length[Z] = float(length_*normal[Z]);
    c[X] = x; c[Y] = y; c[Z] = z;
}
void TriangularPrism::draw() {
    
    glPushMatrix();
    
    // positions the cube correctly
    this->positionInGL();

    // colours correctly
    this->setColorInGL();
    
    float* vc1 = scos::VectorMaths::minus(v1, c);
    float* vc2 = scos::VectorMaths::minus(v2, c);
    float* vc3 = scos::VectorMaths::minus(v3, c);
    
    glBegin(GL_TRIANGLES);
    glVertex3f(vc1[X], vc1[Y], vc1[Z]);
    glVertex3f(vc2[X], vc2[Y], vc2[Z]);
    glVertex3f(vc3[X], vc3[Y], vc3[Z]);
    
    float* f1 = scos::VectorMaths::minus(vc1, length);
    float* f2 = scos::VectorMaths::minus(vc2, length);
    float* f3 = scos::VectorMaths::minus(vc3, length);
    
    glVertex3f(f1[X], f1[Y], f1[Z]);
    glVertex3f(f2[X], f2[Y], f2[Z]);
    glVertex3f(f3[X], f3[Y], f3[Z]);
    glEnd();
    
    glBegin(GL_QUAD_STRIP);
    glVertex3f(vc1[X], vc1[Y], vc1[Z]);
    glVertex3f(f1[X], f1[Y], f1[Z]);
    glVertex3f(vc2[X], vc2[Y], vc2[Z]);
    glVertex3f(f2[X], f2[Y], f2[Z]);
    glVertex3f(vc3[X], vc3[Y], vc3[Z]);
    glVertex3f(f3[X], f3[Y], f3[Z]);
    glEnd();
    
    // draw edges in black
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3f(vc1[X], vc1[Y], vc1[Z]);
    glVertex3f(vc2[X], vc2[Y], vc2[Z]);
    glVertex3f(vc3[X], vc3[Y], vc3[Z]);
    glEnd();
    
    glBegin(GL_LINE_LOOP);
    glVertex3f(f1[X], f1[Y], f1[Z]);
    glVertex3f(f2[X], f2[Y], f2[Z]);
    glVertex3f(f3[X], f3[Y], f3[Z]);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex3f(vc1[X], vc1[Y], vc1[Z]);
    glVertex3f(f1[X], f1[Y], f1[Z]);
    glVertex3f(vc2[X], vc2[Y], vc2[Z]);
    glVertex3f(f2[X], f2[Y], f2[Z]);
    glVertex3f(vc3[X], vc3[Y], vc3[Z]);
    glVertex3f(f3[X], f3[Y], f3[Z]);
    glEnd();
    glPopMatrix();
}

void TriangularPrism::setX(double x_) {
    Shape::setX(x_);
    v1[X] += x_ - c[X];
    v2[X] += x_ - c[X];
    v3[X] += x_ - c[X];
    c[X] = x_;
};

void TriangularPrism::setY(double y_) {
    Shape::setY(y_);
    v1[Y] += y_ - c[Y];
    v2[Y] += y_ - c[Y];
    v3[Y] += y_ - c[Y];
    c[Y] = y_;
};

void TriangularPrism::setZ(double z_) {
    Shape::setZ(z_);
    v1[Z] += z_ - c[Z];
    v2[Z] += z_ - c[Z];
    v3[Z] += z_ - c[Z];
    c[Z] = z_;
};

void TriangularPrism::setPosition(double x_, double y_, double z_) {
    this->setX(x_);
    this->setY(y_);
    this->setZ(z_);
};

double TriangularPrism::findCentroidX(float *v1_, float *v2_, float *v3_, double length_) {
    float c[3] = {(v1_[X]+v2_[X]+v3_[X])/3, (v1_[Y]+v2_[Y]+v3_[Y])/3, (v1_[Z]+v2_[Z]+v3_[Z])/3};
    float* v12 = scos::VectorMaths::minus(v2_, v1_);
    float* v13 = scos::VectorMaths::minus(v3_, v1_);
    float* normal = scos::VectorMaths::cross(v12, v13);
    scos::VectorMaths::normalise(normal);
    // multiple of length in the normal direction
    float length[3];
    // each term is divided by two because we ant the middle of the prism
    length[X] = float(length_*normal[X]/2); length[Y] = float(length_*normal[Y]/2); length[Z] = float(length_*normal[Z]/2);
    float* centroid = scos::VectorMaths::minus(c, length);
    return centroid[X];
}

double TriangularPrism::findCentroidY(float *v1_, float *v2_, float *v3_, double length_) {
    float c[3] = {(v1_[X]+v2_[X]+v3_[X])/3, (v1_[Y]+v2_[Y]+v3_[Y])/3, (v1_[Z]+v2_[Z]+v3_[Z])/3};
    float* v12 = scos::VectorMaths::minus(v2_, v1_);
    float* v13 = scos::VectorMaths::minus(v3_, v1_);
    float* normal = scos::VectorMaths::cross(v12, v13);
    scos::VectorMaths::normalise(normal);
    // multiple of length in the normal direction
    float length[3];
    // each term is divided by two because we ant the middle of the prism
    length[X] = float(length_*normal[X]/2); length[Y] = float(length_*normal[Y]/2); length[Z] = float(length_*normal[Z]/2);
    float* centroid = scos::VectorMaths::minus(c, length);
    return centroid[Y];
}

double TriangularPrism::findCentroidZ(float *v1_, float *v2_, float *v3_, double length_) {
    float c[3] = {(v1_[X]+v2_[X]+v3_[X])/3, (v1_[Y]+v2_[Y]+v3_[Y])/3, (v1_[Z]+v2_[Z]+v3_[Z])/3};
    float* v12 = scos::VectorMaths::minus(v2_, v1_);
    float* v13 = scos::VectorMaths::minus(v3_, v1_);
    float* normal = scos::VectorMaths::cross(v12, v13);
    scos::VectorMaths::normalise(normal);
    // multiple of length in the normal direction
    float length[3];
    // each term is divided by two because we ant the middle of the prism
    length[X] = float(length_*normal[X]/2); length[Y] = float(length_*normal[Y]/2); length[Z] = float(length_*normal[Z]/2);
    float* centroid = scos::VectorMaths::minus(c, length);
    return centroid[Z];
}

 
