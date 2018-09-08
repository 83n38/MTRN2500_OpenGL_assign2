//
//  RemoteVehicle.hpp
//  MTRN2500_OpenGL_assign2
//
//  Created by Ben Henderson on 8/9/18.
//  Copyright © 2018 Ben Henderson. All rights reserved.
//

#ifndef RemoteVehicle_hpp
#define RemoteVehicle_hpp

#include "Vehicle.hpp"
#include <stdio.h>

#include "RectangularPrism.hpp"
#include "TriangularPrism.hpp"
#include "TrapezoidalPrism.hpp"
#include "Cylinder.hpp"
#include "Messages.hpp"
#include <stdio.h>

class RemoteVehicle : public Vehicle {
public:
    RemoteVehicle(VehicleModel vm);
    
    void draw();

};

#endif /* RemoteVehicle_hpp */
