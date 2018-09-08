//
//  SnowPlowVehicle.cpp
//
//
//  Created by Ben Henderson on 2/9/18.
//

#include "SnowPlowVehicle.hpp"

#include <iterator>

SnowPlowVehicle::SnowPlowVehicle() {
    float v1[] = {1.5f, 2.f, -2.f};
    float v2[] = {0.f, 3.5f, -2.f};
    float v3[] = {-2.f, 3.5f, -2.f};
    float v4[] = {-3.f, 2.f, -2.f};
    window = new TrapezoidalPrism(v1, v2, v3, v4, 4);
    
    //position for wheels
    LRwheel = new Cylinder(-3.0, 0.5, -2.25, 1, 0.5);
    RRwheel = new Cylinder(-3.0, 0.5, 2.25, 1, 0.5);
    LFwheel = new Cylinder(3.0, 0.25, -2.25, 0.75, 0.5);
    RFwheel = new Cylinder(3.0, 0.25, 2.25, 0.75, 0.5);
    
    //position for vehicle body
    body = new RectangularPrism(0, 1, 0, 6, 2, 4);
    
    //position for front diffuser
    float w1[] = {3.f, 0.f, -2.f};
    float w2[] = {3.f, 1.f, -2.f};
    float w3[] = {5.f, 0.f, -2.f};
    frontdiffuser = new TriangularPrism(w1, w2, w3, 4);
    
    // get the offset from the vehicles X, Y, and Z
    this->getOffsets();
    
    body->draw();
    frontdiffuser->draw();
    LRwheel->draw();
    RRwheel->draw();
    LFwheel->draw();
    RFwheel->draw();
    window->draw();
    
    this->addShape(body);
    this->addShape(frontdiffuser);
    this->addShape(window);
    this->addShape(LRwheel);
    this->addShape(RRwheel);
    this->addShape(LFwheel);
    this->addShape(RFwheel);
}

void SnowPlowVehicle::draw() {
    
    std::vector<Shape *>::iterator it;
    
    // we push the current coordinates
    glPushMatrix();
    
    // now rotate relative to the vehicle
    // all components to the correct rotation and position
    this->positionInGL();
    
    // add steering
    LFwheel->setRotation(steering);
    RFwheel->setRotation(steering);
    
    for (it = shapes.begin(); it != shapes.end(); ++it) {
        
        (*it)->draw();
    }
    
    // now pop the matrix, back to original coordinates
    glPopMatrix();
    
    
}

void SnowPlowVehicle::getOffsets() {
    // window offsets
    windowX = window->getX();
    windowY = window->getY();
    windowZ = window->getZ();
    
    // wheel offsets
    xRwheelX = LRwheel->getX();
    xRwheelY = LRwheel->getY();
    xFwheelX = RFwheel->getX();
    xFwheelY = RFwheel->getY();
    LxwheelZ = LRwheel->getZ();
    RxwheelZ = RFwheel->getZ();
    
    // body offsets
    bodyX = body->getX();
    bodyY = body->getY();
    bodyZ = body->getZ();
    
    // diffuser offsets
    frontdiffuserX = frontdiffuser->getX();
    frontdiffuserY = frontdiffuser->getY();
    frontdiffuserZ = frontdiffuser->getZ();
}

