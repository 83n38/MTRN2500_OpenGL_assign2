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
    TrapezoidalPrism* window = new TrapezoidalPrism(v1, v2, v3, v4, 4);
    
    //position for wheels
    Cylinder* LRwheel = new Cylinder(-3.0, 0.5, -2.25, 1, 0.5);
    Cylinder* RRwheel = new Cylinder(-3.0, 0.5, 2.25, 1, 0.5);
    Cylinder* LFwheel = new Cylinder(3.0, 0.25, -2.25, 0.75, 0.5);
    Cylinder* RFwheel = new Cylinder(3.0, 0.25, 2.25, 0.75, 0.5);
    
    //position for vehicle body
    RectangularPrism* body = new RectangularPrism(0, 1, 0, 6, 2, 4);
    
    //position for front diffuser
    float w1[] = {3.f, 0.f, -2.f};
    float w2[] = {3.f, 1.f, -2.f};
    float w3[] = {5.f, 0.f, -2.f};
    TriangularPrism* frontdiffuser = new TriangularPrism(w1, w2, w3, 4);
    
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
    for (it = shapes.begin(); it != shapes.end(); ++it)
    {
        (*it)->draw();
    }
    //    for (int i = 0; i < 4; i++) {
    //        shapes[i]->draw();
    //    }
    //    window.draw();
    //    plow.draw();
    //    shaft.draw();
    
    
}

