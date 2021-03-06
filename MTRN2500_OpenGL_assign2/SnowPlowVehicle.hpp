//
//  SnowPlowVehicle.hpp
//  
//
//  Created by Ben Henderson on 2/9/18.
//

#ifndef SnowPlowVehicle_hpp
#define SnowPlowVehicle_hpp

#include "Vehicle.hpp"
#include "RectangularPrism.hpp"
#include "TriangularPrism.hpp"
#include "TrapezoidalPrism.hpp"
#include "Cylinder.hpp"
#include "Messages.hpp"
#include <stdio.h>

class SnowPlowVehicle : public Vehicle {
public:
    SnowPlowVehicle();
    SnowPlowVehicle(VehicleModel vm);
    
    void draw();
private:
    // shapes
    TrapezoidalPrism* window;
    RectangularPrism* body;
    RectangularPrism* LRrim;
    RectangularPrism* LFrim;
    RectangularPrism* RRrim;
    RectangularPrism* RFrim;
    TriangularPrism* frontdiffuser;
    Cylinder* LRwheel;
    Cylinder* RRwheel;
    Cylinder* LFwheel;
    Cylinder* RFwheel;
};

#endif /* SnowPlowVehicle_hpp */
