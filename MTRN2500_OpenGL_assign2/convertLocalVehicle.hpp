//
//  convertLocalVehicle.h
//  MTRN2500_OpenGL_assign2
//
//  Created by Ben Henderson on 9/9/18.
//  Copyright © 2018 Ben Henderson. All rights reserved.
//

#ifndef convertLocalVehicle_hpp
#define convertLocalVehicle_hpp

#include <stdio.h>
#include "Messages.hpp"
#include "Shape.hpp"
#include <math.h>

namespace Convert {
    
    struct ShapeInit* convertRect(struct ShapeInit* obj, double x, double y, double z, double xlen, double ylen, double zlen, float* color);
    
    struct ShapeInit* convertTrap(struct ShapeInit* obj, float* v1, float* v2, float* v3, float* v4, double len, float* color);
    
    struct ShapeInit* convertTri(struct ShapeInit* obj, float* v1, float* v2, float* v3, double len, float* color);
    
    struct ShapeInit* convertCyl(struct ShapeInit* obj, float x, float y, float z, float radius, float len, float* color, bool steering, bool rolling);
};


#endif /* convertLocalVehicle_hpp */
