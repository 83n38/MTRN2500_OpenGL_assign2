//
//  RemoteVehicle.cpp
//  MTRN2500_OpenGL_assign2
//
//  Created by Ben Henderson on 8/9/18.
//  Copyright © 2018 Ben Henderson. All rights reserved.
//

#include "RemoteVehicle.hpp"

// if an object is likely to be a rim, (steering and rolling with the wheel) this returns a pointer
// to the cylinder that is likely it's wheel
static ShapeInit* isRim(VehicleModel* vm, std::vector<ShapeInit>::iterator it) {
    float tolerance = 0.25f; // used to see if the rim is "close enough" to the wheel
    
    std::vector<ShapeInit>::iterator shape;
    // for all the shapes that are part of the model
    
    // vector of all the possibilities
    std::vector<ShapeInit> possible;
    
    // copy the shapes vector
    std::vector<ShapeInit> shapes = vm->shapes;
    
    shape = shapes.begin();

    for (shape = shapes.begin(); shape != shapes.end(); ++shape) {
        // look for one that is a cylinder that is rolling or steering
        if (shape->type == CYLINDER) {
            if (shape->params.cyl.isRolling || shape->params.cyl.isSteering) {
                // get the z value of both ends of the cylinder
                float ends[] = {shape->xyz[2] - shape->params.cyl.depth/2, shape->xyz[2] + shape->params.cyl.depth/2};
                
                // if this object (it) is touching this cylinder (shape) then it's likely to be a rim
                switch (it->type) {
                    case RECTANGULAR_PRISM: {
                        float itEnds[] = {it->xyz[2] + it->params.rect.zlen/2, it->xyz[2] - it->params.rect.zlen/2};
                        // see if either end of the cylinder (wheel) is close to this object's ends in z
                        for (int i = 0; i < sizeof(ends)/sizeof(ends[0]); i++) {
                            for (int j = 0; j < sizeof(itEnds)/sizeof(itEnds[0]); j++) {
                                if (abs(ends[i] - itEnds[j]) <= tolerance) {
                                    // and if the object is within the cylinder's radius in x and y
                                    if (abs(shape->xyz[0]) + shape->params.cyl.radius - abs(it->xyz[0])) {
                                        // within radius in x
                                        if (abs(shape->xyz[1]) + shape->params.cyl.radius - abs(it->xyz[1])) {
                                            // then this cylinder (shape) is possibly a wheel for it
                                            possible.push_back(*shape);
                                            i = 2;
                                            j = 2;
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        break;
                    }
                        
                    case TRIANGULAR_PRISM: {
                        float itEnds[] = {it->xyz[2] + it->params.tri.depth/2, it->xyz[2] - it->params.tri.depth/2};
                        // see if either end of the cylinder (wheel) is close to this object's ends in z
                        for (int i = 0; i < sizeof(ends)/sizeof(ends[0]); i++) {
                            for (int j = 0; j < sizeof(itEnds)/sizeof(itEnds[0]); j++) {
                                if (abs(ends[i] - itEnds[j]) <= tolerance) {
                                    // and if the object is within the cylinder's radius in x and y
                                    if (abs(shape->xyz[0]) + shape->params.cyl.radius - abs(it->xyz[0])) {
                                        // within radius in x
                                        if (abs(shape->xyz[1]) + shape->params.cyl.radius - abs(it->xyz[1])) {
                                            // then this cylinder (shape) is possibly a wheel for it
                                            possible.push_back(*shape);
                                            i = 2;
                                            j = 2;
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        break;
                    }
                        
                    case TRAPEZOIDAL_PRISM: {
                        float itEnds[] = {it->xyz[2] + it->params.trap.depth/2, it->xyz[2] - it->params.trap.depth/2};
                        // see if either end of the cylinder (wheel) is close to this object's ends in z
                        for (int i = 0; i < sizeof(ends)/sizeof(ends[0]); i++) {
                            for (int j = 0; j < sizeof(itEnds)/sizeof(itEnds[0]); j++) {
                                if (abs(ends[i] - itEnds[j]) <= tolerance) {
                                    // and if the object is within the cylinder's radius in x and y
                                    if (abs(shape->xyz[0]) + shape->params.cyl.radius - abs(it->xyz[0])) {
                                        // within radius in x
                                        if (abs(shape->xyz[1]) + shape->params.cyl.radius - abs(it->xyz[1])) {
                                            // then this cylinder (shape) is possibly a wheel for it
                                            possible.push_back(*shape);
                                            i = 2;
                                            j = 2;
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        break;
                    }

    /* I don't think cylinders will be rims, + we won't call the function if it->type == CYLINDER
                    case CYLINDER: {
                        float itEnds[] = {it->xyz[2] + it->params.cyl.depth/2, it->xyz[2] - it->params.cyl.depth/2};
                        // see if either end of the cylinder (wheel) is close to this object's ends
                        for (int i = 0; i < sizeof(ends)/sizeof(ends[0]); i++) {
                            for (int j = 0; j < sizeof(itEnds)/sizeof(itEnds[0]); j++) {
                                if (abs(ends[i] - itEnds[j]) <= tolerance) {
                                    // then this cylinder is possibly a wheel for it
                                    possible.push_back(*shape);
                                     i = 2;
                                     j = 2;
                                     break;
                                }
                            }
                        }
                        break;
                    }
     */
                        
                    default: {
                        std::cout << "Recieved an UNKNOWN_SHAPE" << std::endl;
                        break;
                    }
                } // end nested ifs
            } // end switch
        } // end checking if shape is a cylinder
    } // end for, looping through all the shapes
    // now we've checked all the shapes to see how many are possibly wheels that this object could be "attached" to
    // rims should only be attacjed to 1 wheel
    if (possible.size() == 1) {
        // we're going to return the radius of the wheel that this object is attached too
        std::cout << "We found a rim for a wheel!!" << std::endl;
        return &possible.front();
    } else {
        return NULL;
    }
}

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
                
                // check to see if this might be a rim to a wheel
                ShapeInit* wheel = isRim(&vm, it);
                if (wheel != NULL) {
                    // then this is a rim!
                    // so copy the parameters of the wheel it's a rim to
                    rect->setIsSteering(wheel->params.cyl.isSteering);
                    rect->setIsRolling(wheel->params.cyl.isRolling);
                    rect->setRadiusOfRoll(wheel->params.cyl.radius);
                }
                rect->setColor(it->rgb[0], it->rgb[1], it->rgb[2]);
                rect->draw();
                this->addShape(rect);
                break;
            }
                
            case TRIANGULAR_PRISM: {
                struct ShapeParameter::TriangularParameters* data = &(it->params.tri);
                float v1[] = {it->xyz[0] - data->alen, it->xyz[1], it->xyz[2] - data->depth/2};
                float v2[] = {static_cast<float>(it->xyz[0] - data->alen + data->blen*cos(data->angle*M_PI/180.f)), static_cast<float>(it->xyz[1] + data->blen*sin(data->angle*M_PI/180.f)), it->xyz[2] - data->depth/2};
                float v3[] = {it->xyz[0], it->xyz[1], it->xyz[2] - data->depth/2};
                TriangularPrism* tri = new TriangularPrism(v1, v2, v3, data->depth, it->rotation);
                
                // check to see if this might be a rim to a wheel
                ShapeInit* wheel = isRim(&vm, it);
                if (wheel != NULL) {
                    // then this is a rim!
                    tri->setIsSteering(wheel->params.cyl.isSteering);
                    tri->setIsRolling(wheel->params.cyl.isRolling);
                    tri->setRadiusOfRoll(wheel->params.cyl.radius);
                }
                tri->setColor(it->rgb[0], it->rgb[1], it->rgb[2]);
                tri->draw();
                this->addShape(tri);
                break;
            }
                
            case TRAPEZOIDAL_PRISM: {
                struct ShapeParameter::TrapezoidalParameters* data = &(it->params.trap);
                float v1[] = {it->xyz[0] - data->alen/2, it->xyz[1], it->xyz[2] + data->depth/2};
                float v2[] = {it->xyz[0] - data->alen/2 + data->aoff, it->xyz[1] + data->height, it->xyz[2] + data->depth/2};
                float v3[] = {it->xyz[0] - data->alen/2 + data->aoff + data->blen, it->xyz[1] + data->height, it->xyz[2] + data->depth/2};
                float v4[] = {it->xyz[0] + data->alen/2, it->xyz[1], it->xyz[2] + data->depth/2};
                TrapezoidalPrism* trap = new TrapezoidalPrism(v1, v2, v3, v4, data->depth, it->rotation);
                
                // check to see if this might be a rim to a wheel
                ShapeInit* wheel = isRim(&vm, it);
                if (wheel != NULL) {
                    // then this is a rim!
                    trap->setIsSteering(wheel->params.cyl.isSteering);
                    trap->setIsRolling(wheel->params.cyl.isRolling);
                    trap->setRadiusOfRoll(wheel->params.cyl.radius);
                }
                trap->setColor(it->rgb[0], it->rgb[1], it->rgb[2]);
                trap->draw();
                this->addShape(trap);
                break;
            }
                
            case CYLINDER: {
                struct ShapeParameter::CylinderParameters* data = &(it->params.cyl);
                Cylinder* cyl = new Cylinder(it->xyz[0], it->xyz[1] + data->radius, it->xyz[2], data->radius, data->depth, it->rotation);
                cyl->setColor(it->rgb[0], it->rgb[1], it->rgb[2]);

                cyl->setIsSteering(data->isSteering);
                cyl->setIsRolling(data->isRolling);
                if (cyl->getIsRolling()) {
                    cyl->setRadiusOfRoll(data->radius);
                }
                
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
        if ((*it)->getIsSteering()) {
            (*it)->setRotation(steering);
        }
        
        // add rolling
        if ((*it)->getIsRolling()) {
            //std::cout << "Something remote is rolling, hopefully sometimes it's blue? " << (*it)->getBlue() << std::endl;
            //std::cout << "Speed: " << this->getSpeed() << std::endl;
            double rollAngle = (*it)->getRoll(); // get the previous roll angle
            //std::cout << "Previous roll angle: " << rollAngle << std::endl;
            rollAngle -= this->getSpeed()/(*it)->getRadiusOfRoll(); // update the roll angle depending on the size of the wheel
            // smaller wheels should roll quicker
            if ((*it)->getBlue() == 1.f) {
                //std::cout << "roll angle: " << rollAngle << std::endl;
            }
            (*it)->setRoll(rollAngle);
        }
        (*it)->draw();
    }
    
    // now pop the matrix, back to original coordinates
    glPopMatrix();
}
