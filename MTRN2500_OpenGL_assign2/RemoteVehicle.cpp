//
//  RemoteVehicle.cpp
//  MTRN2500_OpenGL_assign2
//
//  Created by Ben Henderson on 8/9/18.
//  Copyright © 2018 Ben Henderson. All rights reserved.
//

#include "RemoteVehicle.hpp"

// constructor for remote vehicles recived from messages from the interwebs
RemoteVehicle::RemoteVehicle(VehicleModel vm) {
    // loop through all the shapes that are in this model
    std::vector<ShapeInit>::iterator it;
    for (it = vm.shapes.begin(); it != vm.shapes.end(); ++it) {
        switch (it->type) {
            case RECTANGULAR_PRISM: {
                // just a pointer to the rectangular parameters struct so I don't have to type it all out
                struct ShapeParameter::RectangularParameters* data = &(it->params.rect);
                RectangularPrism* rect = new RectangularPrism(it->xyz[0], it->xyz[1]+data->ylen/2, it->xyz[2], data->xlen, data->ylen, data->zlen, it->rotation);
                rect->setColor(it->rgb[0], it->rgb[1], it->rgb[2]);
                rect->draw();
                this->addShape(rect);
                break;
            }
                
            case TRIANGULAR_PRISM: {
                struct ShapeParameter::TriangularParameters* data = &(it->params.tri);
                float v1[] = {it->xyz[0] - data->alen/2, it->xyz[1], it->xyz[2] - data->depth/2};
                float v2[] = {static_cast<float>(it->xyz[0] - data->alen/2 + data->blen*cos(data->angle*M_PI/180.f)), static_cast<float>(it->xyz[1] + data->blen*sin(data->angle*M_PI/180.f)), it->xyz[2] - data->depth/2};
                float v3[] = {it->xyz[0] + data->alen/2, it->xyz[1], it->xyz[2] - data->depth/2};
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
                Cylinder* cyl = new Cylinder(it->xyz[0], it->xyz[1] + data->radius, it->xyz[2], data->radius, data->depth, it->rotation);
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

void RemoteVehicle::draw() {
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
