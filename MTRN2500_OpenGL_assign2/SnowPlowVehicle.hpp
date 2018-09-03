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
#include <stdio.h>

class SnowPlowVehicle : public Vehicle {
public:
    SnowPlowVehicle();
    
    void draw();
};

#endif /* SnowPlowVehicle_hpp */
