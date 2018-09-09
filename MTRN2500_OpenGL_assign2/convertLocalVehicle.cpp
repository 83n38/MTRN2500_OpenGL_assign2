//
//  convertLocalVehicle.cpp
//  MTRN2500OpenGLassign2
//
//  Created by Ben Henderson on 9/9/18.
//  Copyright © 2018 Ben Henderson. All rights reserved.
//

#include <stdio.h>
#include "convertLocalVehicle.hpp"

namespace Convert {
    
    struct ShapeInit* convertRect(struct ShapeInit* obj, double x, double y, double z, double xlen, double ylen, double zlen, float* color) {
        obj->type =  RECTANGULAR_PRISM;
        obj->xyz[0] = x;
        obj->xyz[1] = y - ylen/2;
        obj->xyz[2] = z;
        obj->rotation = 0;
        obj->rgb[0] = color[0];
        obj->rgb[1] = color[1];
        obj->rgb[2] = color[2];
        obj->params.rect.xlen = xlen;
        obj->params.rect.ylen = ylen;
        obj->params.rect.zlen = zlen;
        return obj;
    }
    
    struct ShapeInit* convertTrap(struct ShapeInit* obj, float* v1, float* v2, float* v3, float* v4, double len, float* color) {
        /*
         obj stuff
         float v1[] = {1.5f, 2.f, -2.f};
         float v2[] = {0.f, 3.5f, -2.f};
         float v3[] = {-2.f, 3.5f, -2.f};
         float v4[] = {-3.f, 2.f, -2.f};
         obj = new TrapezoidalPrism(v1, v2, v3, v4, 4);
         */
        obj->type =  TRAPEZOIDAL_PRISM;
        obj->xyz[0] = (v1[0] - v4[0])/2 + v4[0];
        obj->xyz[1] = v1[1];
        obj->xyz[2] = v1[2] + len/2;
        obj->rotation = 0;
        obj->rgb[0] = color[0];
        obj->rgb[1] = color[1];
        obj->rgb[2] = color[2];
        obj->params.trap.alen = (v1[0] - v4[0]);
        obj->params.trap.blen = v2[0] - v3[0];
        obj->params.trap.height = v2[1] - v1[1];
        obj->params.trap.aoff = v3[0] - v4[0];
        obj->params.trap.depth = len;
        return obj;
    }
    
    struct ShapeInit* convertTri(struct ShapeInit* obj, float* v1, float* v2, float* v3, double len, float* color) {
        /*
         position for front diffuser
         float w1[] = {3.f, 0.f, -2.f};
         float w2[] = {3.f, 1.f, -2.f};
         float w3[] = {5.f, 0.f, -2.f};
         frontdiffuser = new TriangularPrism(w1, w2, w3, 4);
         */
        obj->type =  TRIANGULAR_PRISM;
        obj->xyz[0] = v3[0];
        obj->xyz[1] = v1[1];
        obj->xyz[2] = v1[2] + len/2;
        obj->rotation = 0;
        obj->rgb[0] = color[0];
        obj->rgb[1] = color[1];
        obj->rgb[2] = color[2];
        obj->params.tri.alen = (v3[0] - v1[0]);
        obj->params.tri.blen = sqrt(pow(v2[0] - v1[0], 2) + pow(v2[1] - v1[1], 2));
        obj->params.tri.angle = atan((v2[1] - v1[1])/(v2[0] - v1[0])) * 180 / M_PI;
        obj->params.tri.depth = len;
        return obj;
    }
    
    struct ShapeInit* convertCyl(struct ShapeInit* obj, float x, float y, float z, float radius, float len, float* color, bool steering, bool rolling) {
        //Cylinder* cyl = new Cylinder(it->xyz[0], it->xyz[1] + data->radius, it->xyz[2], data->radius, data->depth, it->rotation);
        obj->type =  CYLINDER;
        obj->xyz[0] = x;
        obj->xyz[1] = y - radius;
        obj->xyz[2] = z;
        obj->rotation = 0;
        obj->rgb[0] = color[0];
        obj->rgb[1] = color[1];
        obj->rgb[2] = color[2];
        obj->params.cyl.depth = len;
        obj->params.cyl.radius = radius;
        obj->params.cyl.isRolling = rolling;
        obj->params.cyl.isSteering = steering;
        return obj;
    }
};
