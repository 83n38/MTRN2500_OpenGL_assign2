
#include "Shape.hpp"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#elif defined(WIN32)
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <iostream>


Shape::Shape() {
	x = y = z = 0.0;
	rotation = 0.0;
	red = green = blue = 1.0;
    isSteering = false;
    isRolling = false;
    radiusOfRoll = 1;
    roll = 0;
};

Shape::Shape(double x_, double y_, double z_) {
	x = x_;
	y = y_;
	z = z_;

	rotation = 0.0;
	red = green = blue = 1.0;
    isSteering = false;
    isRolling = false;
    radiusOfRoll = 1;
    roll = 0;
};

Shape::Shape(double x_, double y_, double z_, double rotation_) {

	x = x_;
	y = y_;
	z = z_;

	rotation = rotation_;
	red = green = blue = 1.0;
    isSteering = false;
    isRolling = false;
    isRolling = false;
    radiusOfRoll = 1;
    roll = 0;
};

Shape::~Shape() {
}

double Shape::getX() {
	return x;
};

double Shape::getY() {
	return y;
};

double Shape::getZ() {
	return z;
};

double Shape::getRotation() {
	return rotation;
};

double Shape::getRoll() {
    return roll;
}

void Shape::setX(double x_) {
	x = x_;
};

void Shape::setY(double y_) {
	y = y_;
};

void Shape::setZ(double z_) {
	z = z_;
};

void Shape::setRotation(double rotation_) {
	rotation = rotation_;
};

void Shape::setRoll(double roll_) {
    roll = roll_;
    
    while (roll > 360) roll -= 360;
    while (roll < 0) roll += 360;
    
}

void Shape::setIsRolling(bool b) {
    isRolling = b;
}
void Shape::setIsSteering(bool b) {
    isSteering = b;
}
void Shape::setRadiusOfRoll(double r) {
    radiusOfRoll = r;
}

bool Shape::getIsRolling() {
    return isRolling;
}
bool Shape::getIsSteering() {
    return isSteering;
}
double Shape::getRadiusOfRoll() {
    return radiusOfRoll;
}


void Shape::setPosition(double x_, double y_, double z_) {
	x = x_;
	y = y_;
	z = z_;
};

void Shape::positionInGL() {
	glTranslated(x, y, z);
	glRotated(-rotation, 0, 1, 0);
    if (isRolling) {
        glRotated(roll, 0, 0, 1);
//        if (this->blue == 1.f) {
//            std::cout << roll << std::endl;
//        }
    }
};

void Shape::setColorInGL() {
	glColor3f(red, green, blue);
};

double Shape::getRed() {
	return red;
};

double Shape::getGreen() {
	return green;
};

double Shape::getBlue() {
	return blue;
};

void Shape::setColor(float red_, float green_, float blue_) {
	red = red_;
	green = green_;
	blue = blue_;
};

