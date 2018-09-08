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
    
    //
    
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

// constructor for remote vehicles recived from messages from the interwebs
SnowPlowVehicle::SnowPlowVehicle(VehicleModel vm) {
    // loop through all the shapes that are in this model
    std::vector<ShapeInit>::iterator it;
    for (it = vm.shapes.begin(); it != vm.shapes.end(); ++it) {
        switch (it->type) {
            case RECTANGULAR_PRISM: {
                // just a pointer to the rectangular parameters struct so I don't have to type it all out
                struct ShapeParameter::RectangularParameters* data = &(it->params.rect);
                RectangularPrism* rect = new RectangularPrism(it->xyz[0], it->xyz[0], it->xyz[0], data->xlen, data->ylen, data->zlen, it->rotation);
                rect->setColor(it->rgb[0], it->rgb[1], it->rgb[2]);
                rect->draw();
                this->addShape(rect);
                break;
            }
                
            case TRIANGULAR_PRISM: {
                struct ShapeParameter::TriangularParameters* data = &(it->params.tri);
                float v1[] = {it->xyz[0], it->xyz[1], it->xyz[2]};
                float v2[] = {static_cast<float>((it->xyz[0] - data->blen*cos(data->angle*M_PI/180.f))), static_cast<float>((it->xyz[1] + data->blen*sin(data->angle*M_PI/180.f))), it->xyz[2]};
                float v3[] = {it->xyz[0] + data->alen, it->xyz[1], it->xyz[2]};
                TriangularPrism* tri = new TriangularPrism(v1, v2, v3, data->depth, it->rotation);
                tri->setColor(it->rgb[0], it->rgb[1], it->rgb[2]);
                tri->draw();
                this->addShape(tri);
                break;
            }
                
            case TRAPEZOIDAL_PRISM: {
                std::cout << "Can't be bothered to actually make one" << std::endl;
                break;
            }
                
            case CYLINDER: {
                struct ShapeParameter::CylinderParameters* data = &(it->params.cyl);
                Cylinder* cyl = new Cylinder(it->xyz[0], it->xyz[0], it->xyz[0], data->radius, data->depth, it->rotation);
                cyl->setColor(it->rgb[0], it->rgb[1], it->rgb[2]);
                cyl->draw();
                this->addShape(cyl);
                break;
            }
                
            default: {
                std::cout << "Recieved an UNKNOWN_SHAPE" << std::endl;
                break;
            }
        }
    }
    
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

