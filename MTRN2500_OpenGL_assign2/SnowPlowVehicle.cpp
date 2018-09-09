//
//  SnowPlowVehicle.cpp
//
//
//  Created by Ben Henderson on 2/9/18.
//

#include "SnowPlowVehicle.hpp"

#include <iterator>
#include <math.h>

SnowPlowVehicle::SnowPlowVehicle() {
    float v1[] = {1.5f, 2.f, -2.f};
    float v2[] = {0.f, 3.5f, -2.f};
    float v3[] = {-2.f, 3.5f, -2.f};
    float v4[] = {-3.f, 2.f, -2.f};
    window = new TrapezoidalPrism(v1, v2, v3, v4, 4);
    
    //position for rims
    
    LRrim = new RectangularPrism(-3.0, 0.5, -2.25, 0.5, 0.5, 0.6);
    RRrim = new RectangularPrism(-3.0, 0.5, 2.25, 0.5, 0.5, 0.6);
    LFrim = new RectangularPrism(3.0, 0.25, -2.25, 0.5, 0.5, 0.6);
    RFrim = new RectangularPrism(3.0, 0.25, 2.25, 0.5, 0.5, 0.6);
    
    
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
    
    // set the front wheels and rims to steer
    LFwheel->isSteering = true;
    RFwheel->isSteering = true;
    LFrim->isSteering = true;
    RFrim->isSteering = true;
    
    // all wheels and rims to roll
    LFwheel->isRolling = true;
    RFwheel->isRolling = true;
    LFrim->isRolling = true;
    RFrim->isRolling = true;
    LFwheel->radiusOfRoll = 0.75;
    RFwheel->radiusOfRoll = 0.75;
    LFrim->radiusOfRoll = 0.75;
    RFrim->radiusOfRoll = 0.75;
    
    LRwheel->isRolling = true;
    RRwheel->isRolling = true;
    LRrim->isRolling = true;
    RRrim->isRolling = true;
    LRwheel->radiusOfRoll = 1;
    RRwheel->radiusOfRoll = 1;
    LRrim->radiusOfRoll = 1;
    RRrim->radiusOfRoll = 1;
    
    body->draw();
    frontdiffuser->draw();
    LRwheel->draw();
    RRwheel->draw();
    LFwheel->draw();
    RFwheel->draw();
    window->draw();
    LRrim->draw();
    LFrim->draw();
    RRrim->draw();
    RFrim->draw();
    LRrim->setColor(0, 0, 255);
    LFrim->setColor(0, 0, 255);
    RRrim->setColor(0, 0, 255);
    RFrim->setColor(0, 0, 255);
    
    this->addShape(body);
    this->addShape(frontdiffuser);
    this->addShape(window);
    this->addShape(LRwheel);
    this->addShape(RRwheel);
    this->addShape(LFwheel);
    this->addShape(RFwheel);
    this->addShape(LRrim);
    this->addShape(LFrim);
    this->addShape(RRrim);
    this->addShape(RFrim);
}


void SnowPlowVehicle::draw() {
    
    std::vector<Shape *>::iterator it;
    
    // we push the current coordinates
    glPushMatrix();
    
    // now rotate relative to the vehicle
    // all components to the correct rotation and position
    this->positionInGL();
    
    
    
    for (it = shapes.begin(); it != shapes.end(); ++it) {
        // add steering
        if ((*it)->isSteering) {
            (*it)->setRotation(steering);
        }
        
        // add rolling
        if ((*it)->isRolling) {
            double rollAngle = (*it)->getRoll(); // get the previous roll angle
            rollAngle -= this->getSpeed()/(*it)->radiusOfRoll; // update the roll angle depending on the size of the wheel
            // smaller wheels should roll quicker
            (*it)->setRoll(rollAngle);
        }
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

