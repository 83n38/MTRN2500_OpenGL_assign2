//
//  RectangularPrism.cpp
//  
//
//  Created by Ben Henderson on 1/9/18.
//

#include "RectangularPrism.hpp"

RectangularPrism::RectangularPrism() {
    x_length = y_length = z_length = 1;
}

RectangularPrism::RectangularPrism(double x_, double y_, double z_, double x_length_, double y_length_, double z_length_)
: Shape(x_, y_, z_)
{
    x_length = x_length_;
    y_length = y_length_;
    z_length = z_length_;
}

RectangularPrism::RectangularPrism(double x_, double y_, double z_, double x_length_, double y_length_, double z_length_, double rotation_)
: Shape(x_, y_, z_, rotation_)
{
    x_length = x_length_;
    y_length = y_length_;
    z_length = z_length_;
}

void RectangularPrism::draw() {
    
    glPushMatrix();
    
    // positions the cube correctly
    this->positionInGL();
    
    // sizes the cube correctly
    glScalef(x_length, y_length, z_length);
    
    // colours correctly
    this->setColorInGL();
    
    glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
    // Top face (y = 0.5f)
    // Define vertices in counter-clockwise (CCW) order with normal pointing out
    glVertex3f( 0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f,  0.5f);
    glVertex3f( 0.5f, 0.5f,  0.5f);
    
    // Bottom face (y = -0.5f)
    glVertex3f( 0.5f, -0.5f,  0.5f);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f, -0.5f, -0.5f);
    
    // Front face  (z = 0.5f)
    glVertex3f( 0.5f,  0.5f, 0.5f);
    glVertex3f(-0.5f,  0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f( 0.5f, -0.5f, 0.5f);
    
    // Back face (z = -0.5f)
    glVertex3f( 0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f,  0.5f, -0.5f);
    glVertex3f( 0.5f,  0.5f, -0.5f);
    
    // Left face (x = -0.5f)
    glVertex3f(-0.5f,  0.5f,  0.5f);
    glVertex3f(-0.5f,  0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    
    // Right face (x = 0.5f)
    glVertex3f(0.5f,  0.5f, -0.5f);
    glVertex3f(0.5f,  0.5f,  0.5f);
    glVertex3f(0.5f, -0.5f,  0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glEnd();  // End of drawing color-cube
    
    glBegin(GL_LINE_LOOP);            // Draw the edges in black
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3f( 0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f,  0.5f);
    glVertex3f( 0.5f, 0.5f,  0.5f);
    glEnd();
    
    // Bottom face (y = -0.5f)
    glBegin(GL_LINE_LOOP);
    glVertex3f( 0.5f, -0.5f,  0.5f);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f, -0.5f, -0.5f);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex3f( 0.5f, 0.5f, -0.5f);
    glVertex3f( 0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f,  0.5f);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glVertex3f( 0.5f, -0.5f,  0.5f);
    glVertex3f( 0.5f, -0.5f,  0.5f);
    glEnd();
    
    glPopMatrix();
    
    
//    // Render a pyramid consists of 4 triangles
//    //glLoadIdentity();                  // Reset the model-view matrix
//    glTranslatef(-1.5f, 0.0f, -6.0f);  // Move left and into the screen
//
//    glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
//    // Front
//    glColor3f(0.5f, 0.0f, 0.0f);     // Red
//    glVertex3f( 0.0f, 0.5f, 0.0f);
//    glColor3f(0.0f, 0.5f, 0.0f);     // Green
//    glVertex3f(-0.5f, -0.5f, 0.5f);
//    glColor3f(0.0f, 0.0f, 0.5f);     // Blue
//    glVertex3f(0.5f, -0.5f, 0.5f);
//
//    // Right
//    glColor3f(0.5f, 0.0f, 0.0f);     // Red
//    glVertex3f(0.0f, 0.5f, 0.0f);
//    glColor3f(0.0f, 0.0f, 0.5f);     // Blue
//    glVertex3f(0.5f, -0.5f, 0.5f);
//    glColor3f(0.0f, 0.5f, 0.0f);     // Green
//    glVertex3f(0.5f, -0.5f, -0.5f);
//
//    // Back
//    glColor3f(0.5f, 0.0f, 0.0f);     // Red
//    glVertex3f(0.0f, 0.5f, 0.0f);
//    glColor3f(0.0f, 0.5f, 0.0f);     // Green
//    glVertex3f(0.5f, -0.5f, -0.5f);
//    glColor3f(0.0f, 0.0f, 0.5f);     // Blue
//    glVertex3f(-0.5f, -0.5f, -0.5f);
//
//    // Left
//    glColor3f(0.5f,0.0f,0.0f);       // Red
//    glVertex3f( 0.0f, 0.5f, 0.0f);
//    glColor3f(0.0f,0.0f,0.5f);       // Blue
//    glVertex3f(-0.5f,-0.5f,-0.5f);
//    glColor3f(0.0f,0.5f,0.0f);       // Green
//    glVertex3f(-0.5f,-0.5f, 0.5f);
//    glEnd();   // Done drawing the pyramid
    

}
