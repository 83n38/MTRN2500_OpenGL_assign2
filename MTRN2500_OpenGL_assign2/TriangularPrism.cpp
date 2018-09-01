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


TriangularPrism::TriangularPrism() {
    float v1[] = {1, 0.f, 0.f};
    float v2[] = {float(-0.5), 0, float(sqrt(3)/2)};
    float v3[] = {float(-0.5), 0, float(-sqrt(3)/2)};
    float length[] = {0.f, 1.f, 0.f};
    std::cout << "X: " << v1[X] << " Y: " << v1[Y] << " Z: " << v1[Z] << std::endl;
    
}
TriangularPrism::TriangularPrism(double x_, double y_, double z_, float* v1_, float* v2_, float* v3_, double length_)
: Shape(x_, y_, z_)
{
    float v1[] = {v1_[X], v1_[Y], v1_[Z]};
    float v2[] = {v2_[X], v2_[Y], v2_[Z]};
    float v3[] = {v3_[X], v3_[Y], v3_[Z]};
    // need to figure out the plane parrallel to all these points
    // then find unit vector normal to this plane
    // and the length vector will be a scalar multiple of this normal vector
    float* v12 = scos::VectorMaths::minus(v2, v1);
    float* v13 = scos::VectorMaths::minus(v3, v1);
    float* normal = scos::VectorMaths::cross(v12, v13);
    scos::VectorMaths::normalise(normal);
    // multiple of length in the normal direction
    float length[] = {float(length_*normal[X]), float(length_*normal[Y]), float(length_*normal[Z])};
}
TriangularPrism::TriangularPrism(double x_, double y_, double z_, float* v1_, float* v2_, float* v3_, double length_, double rotation_)
: Shape(x_, y_, z_, rotation_)
{
    float v1[] = {v1_[X], v1_[Y], v1_[Z]};
    float v2[] = {v2_[X], v2_[Y], v2_[Z]};
    float v3[] = {v3_[X], v3_[Y], v3_[Z]};
    // need to figure out the plane parrallel to all these points
    // then find unit vector normal to this plane
    // and the length vector will be a scalar multiple of this normal vector
    float* v12 = scos::VectorMaths::minus(v2, v1);
    float* v13 = scos::VectorMaths::minus(v3, v1);
    float* normal = scos::VectorMaths::cross(v12, v13);
    scos::VectorMaths::normalise(normal);
    // multiple of length in the normal direction
    float length[] = {float(length_*normal[X]), float(length_*normal[Y]), float(length_*normal[Z])};
}
void TriangularPrism::draw() {
    std::cout << "X: " << v1[X] << " Y: " << v1[Y] << " Z: " << v1[Z] << std::endl;
    
    // positions the cube correctly
    this->positionInGL();

    // colours correctly
    this->setColorInGL();
    
    glBegin(GL_TRIANGLES);
    glVertex3f(v1[X], v1[Y], v1[Z]);
    glVertex3f(v2[X], v2[Y], v2[Z]);
    glVertex3f(v3[X], v3[Y], v3[Z]);
    
    float* f1 = scos::VectorMaths::plus(v1, length);
    float* f2 = scos::VectorMaths::plus(v2, length);
    float* f3 = scos::VectorMaths::plus(v3, length);
    
    
    glVertex3f(f1[X], f1[Y], f1[Z]);
    glVertex3f(f2[X], f2[Y], f2[Z]);
    glVertex3f(f3[X], f3[Y], f3[Z]);
    glEnd();
}

 
