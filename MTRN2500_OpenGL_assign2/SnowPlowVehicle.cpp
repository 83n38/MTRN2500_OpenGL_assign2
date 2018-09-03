//
//  SnowPlowVehicle.cpp
//  
//
//  Created by Ben Henderson on 2/9/18.
//

#include "SnowPlowVehicle.hpp"

#include <iterator>

SnowPlowVehicle::SnowPlowVehicle() {
    float v1[] = {2.f, 0.f, 1.5f};
    float v2[] = {2.f, 0.f, -1.5f};
    float v3[] = {1.5f, 0.f, 0.5f};
    float v4[] = {1.5f, 0.f, -0.5f};
    TrapezoidalPrism plow = TrapezoidalPrism(v1, v2, v3, v4, 3);
    
    Cylinder shaft = Cylinder(1.5, 0, 1.5, 0.5, 1);
    
    RectangularPrism body = RectangularPrism(-0.75, 2, 0, 2.5, 2, 1.5);
    
    float w1[] = {0.f, 3.f, 1.f};
    float w2[] = {-0.1f, 3.5f, 1.f};
    float w3[] = {-0.5f, 3.f, 1.f};
    TriangularPrism window = TriangularPrism(w1, w2, w3, 2);
    
    body.draw();
    window.draw();
    shaft.draw();
    plow.draw();

    this->addShape(&body);
    this->addShape(&window);
    this->addShape(&plow);
    this->addShape(&shaft);
}

void SnowPlowVehicle::draw() {
    std::vector<Shape *>::iterator it;
    for (it = shapes.begin(); it != shapes.end(); ++it)
    {
        (*it)->draw();
    }
    for (int i = 0; i < 4; i++) {
        shapes[i]->draw();
    }
//    window.draw();
//    plow.draw();
//    shaft.draw();
    
    
}
