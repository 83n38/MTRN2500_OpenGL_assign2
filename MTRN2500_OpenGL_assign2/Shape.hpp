
#ifndef MTRN3500_SHAPE_H
#define MTRN3500_SHAPE_H

// Abstract class
class Shape {

public:
	Shape();
	Shape(double x_, double y_, double z_);
	Shape(double x_, double y_, double z_, double rotation_);
	virtual ~Shape();

	virtual void draw() = 0;

	double getX();
	double getY();
	double getZ();
	double getRotation();
    double getRoll();

	void setX(double x_);
	void setY(double y_);
	void setZ(double z_);
	void setRotation(double rotation_);
    void setRoll(double roll_);

	void setPosition(double x_, double y_, double z_);

	void positionInGL();
	void setColorInGL();

	double getRed();
	double getGreen();
	double getBlue();
	void setColor(float red_, float green_, float blue_);

    bool isRolling;
    bool isSteering;
    double radiusOfRoll;
    
    

protected:
	double x, y, z;               // position
	double rotation;              // heading of the object in the horizontal plane (degrees)
    double roll;                  // roll angle
	float red, green, blue;       // colour of object

};

#endif // for MTRN3500_SHAPE_H
