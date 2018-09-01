//
//  RectangularPrism.hpp
//  
//
//  Created by Ben Henderson on 1/9/18.
//

#ifndef RectangularPrism_hpp
#define RectangularPrism_hpp

#include "Shape.hpp"
#include <stdio.h>


class RectangularPrism : public Shape {
public:
    RectangularPrism();
    RectangularPrism(double x_, double y_, double z_, double x_length_, double y_length_, double z_length_);
    RectangularPrism(double x_, double y_, double z_, double x_length_, double y_length_, double z_length_, double rotation_);
    void draw();
protected:
    double x_length;
    double y_length;
    double z_length;
    
};

#endif /* RectangularPrism_hpp */
