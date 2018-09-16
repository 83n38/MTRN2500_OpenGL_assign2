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
    id = 0;
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
    
    // set the front wheels and rims to steer
    LFwheel->setIsSteering(true);
    RFwheel->setIsSteering(true);
    LFrim->setIsSteering(true);
    RFrim->setIsSteering(true);
    
    // all wheels and rims to roll
    LFwheel->setIsRolling(true);
    RFwheel->setIsRolling(true);
    LFrim->setIsRolling(true);
    RFrim->setIsRolling(true);
    LFwheel->setRadiusOfRoll(0.75);
    RFwheel->setRadiusOfRoll(0.75);
    LFrim->setRadiusOfRoll(0.75);
    RFrim->setRadiusOfRoll(0.75);
    
    LRwheel->setIsRolling(true);
    RRwheel->setIsRolling(true);
    LRrim->setIsRolling(true);
    RRrim->setIsRolling(true);
    LRwheel->setRadiusOfRoll(1);
    RRwheel->setRadiusOfRoll(1);
    LRrim->setRadiusOfRoll(1);
    RRrim->setRadiusOfRoll(1);
    
//    body->draw();
//    frontdiffuser->draw();
//    LRwheel->draw();
//    RRwheel->draw();
//    LFwheel->draw();
//    RFwheel->draw();
//    window->draw();
//    LRrim->draw();
//    LFrim->draw();
//    RRrim->draw();
//    RFrim->draw();

    LRrim->setColor(0.f, 0.f, 1.f);
    LFrim->setColor(0.f, 0.f, 1.f);
    RRrim->setColor(0.f, 0.f, 1.f);
    RFrim->setColor(0.f, 0.f, 1.f);
    body->setColor(1.f, 0.19f, 0.19f);
    frontdiffuser->setColor(1.f, 0.19f, 0.19f);
    window->setColor(0.18f, 0.56f, 1.f);
    
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
    
    //std::cout << "\nshapes has this many items: " << shapes.size();
    //std::cout << "LRwheel: " << LRwheel << "\nRRwheel: " << RRwheel << "\nLFwheel: " << LFwheel << "\nRFwheel: " << RFwheel << std::endl;
    
    
    for (it = shapes.begin(); it != shapes.end(); ++it) {

        // add steering
        if ((*it)->getIsSteering()) {
            (*it)->setRotation(steering);
        }
        
        // add rolling
        if ((*it)->getIsRolling()) {
            double rollAngle = (*it)->getRoll(); // get the previous roll angle
            rollAngle -= this->getSpeed()/(*it)->getRadiusOfRoll(); // update the roll angle depending on the size of the wheel
            // smaller wheels should roll quicker
            (*it)->setRoll(rollAngle);
        }
        
        (*it)->draw();
//        std::cout << "Drew object @X: " << (*it)->getX() << " Y: " <<(*it)->getY() << " Z: " <<(*it)->getZ() << std::endl;
//        std::cout << "Colour R: " << (*it)->getRed() << " G: " <<(*it)->getGreen() << " B: " <<(*it)->getBlue() <<"\n" << std::endl;
    }
    
    // now pop the matrix, back to original coordinates
    glPopMatrix();
    
    
}

