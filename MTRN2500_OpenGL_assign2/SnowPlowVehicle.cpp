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
    
    //
    
    // set components the correct positions
    window->setPosition(windowX + this->x, windowY + this->y, windowZ + this->z);
    body->setPosition(bodyX + this->x, bodyY + this->y, bodyZ + this->z);
    frontdiffuser->setPosition(frontdiffuserX + this->x, frontdiffuserY + this->y, frontdiffuserZ + this->z);
    LRwheel->setPosition(xRwheelX + this->x, xRwheelY + this->y, LxwheelZ + this->z);
    RRwheel->setPosition(xRwheelX + this->x, xRwheelY + this->y, RxwheelZ + this->z);
    LFwheel->setPosition(xFwheelX + this->x, xFwheelY + this->y, LxwheelZ + this->z);
    RFwheel->setPosition(xFwheelX + this->x, xFwheelY + this->y, RxwheelZ + this->z);
    
    // all components to the correct rotation
    std::vector<Shape *>::iterator it;
    for (it = shapes.begin(); it != shapes.end(); ++it) {
        (*it)->setRotation(this->rotation);
    }
    
    // add steering
    LFwheel->setRotation(LFwheel->getRotation() + steering);
    RFwheel->setRotation(RFwheel->getRotation() + steering);
    
    for (it = shapes.begin(); it != shapes.end(); ++it) {
        (*it)->draw();
    }
    //    for (int i = 0; i < 4; i++) {
    //        shapes[i]->draw();
    //    }
    //    window.draw();
    //    plow.draw();
    //    shaft.draw();
    
    
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

