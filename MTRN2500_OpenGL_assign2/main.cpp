
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <map>

#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <GLUT/glut.h>
	#include <unistd.h>
	#include <sys/time.h>
#elif defined(WIN32)
	#include <Windows.h>
	#include <tchar.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glut.h>
#else
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glut.h>
	#include <unistd.h>
	#include <sys/time.h>
#endif


#include "Camera.hpp"
#include "Ground.hpp"
#include "KeyManager.hpp"

#include "Shape.hpp"
#include "RectangularPrism.hpp"
#include "TriangularPrism.hpp"
#include "TrapezoidalPrism.hpp"
#include "Cylinder.hpp"
#include "Vehicle.hpp"
#include "SnowPlowVehicle.hpp"
#include "RemoteVehicle.hpp"

#include "RemoteDataManager.hpp"
#include "Messages.hpp"
#include "HUD.hpp"
#include "ObstacleManager.hpp"

#include "convertLocalVehicle.hpp"

void display();
void reshape(int width, int height);
void idle();

void keydown(unsigned char key, int x, int y);
void keyup(unsigned char key, int x, int y);
void special_keydown(int keycode, int x, int y);
void special_keyup(int keycode, int x, int y);

void mouse(int button, int state, int x, int y);
void dragged(int x, int y);
void motion(int x, int y);

using namespace std;
using namespace scos;

// Used to store the previous mouse location so we
//   can calculate relative mouse movement.
int prev_mouse_x = -1;
int prev_mouse_y = -1;

// vehicle control related variables
Vehicle * vehicle = NULL;
double speed = 0;
double steering = 0;

// default goal location
std::deque<GoalState> goals;

std::map<int, Vehicle *> otherVehicles;

bool isChasing = false;

int frameCounter = 0;

//int _tmain(int argc, _TCHAR* argv[]) {
int main(int argc, char ** argv) {

	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;

	glutInit(&argc, (char**)(argv));
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("MTRN3500 - GL");

	Camera::get()->setWindowDimensions(WINDOW_WIDTH, WINDOW_HEIGHT);

	glEnable(GL_DEPTH_TEST);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	glutKeyboardFunc(keydown);
	glutKeyboardUpFunc(keyup);
	glutSpecialFunc(special_keydown);
	glutSpecialUpFunc(special_keyup);

	glutMouseFunc(mouse);
	glutMotionFunc(dragged);
	glutPassiveMotionFunc(motion);

	// -------------------------------------------------------------------------
	// Please uncomment the following line of code and replace 'MyVehicle'
	//   with the name of the class you want to show as the current 
	//   custom vehicle.
	// -------------------------------------------------------------------------

	vehicle = new SnowPlowVehicle();

    // test shapes
//    float v1[] = {0.f, 3.f, 2.f};
//    float v2[] = {6.f, 0.f, -3.f};
//    float v3[] = {-1.f, 0.f, 3.f};
//    tri = TriangularPrism(v1,v2,v3,10);
//    cyl = Cylinder(0,0,0,1,10);
//    trap = TrapezoidalPrism();
    
	// add test obstacles
	ObstacleManager::get()->addObstacle(Obstacle(10,10, 1));
	ObstacleManager::get()->addObstacle(Obstacle(10,-10, 1));
	ObstacleManager::get()->addObstacle(Obstacle(-10,10, 1));
	ObstacleManager::get()->addObstacle(Obstacle(-10,-10, 1));

	// add test goal
	GoalState g;
	g.x = 25;
	g.z = 0;
	goals.push_back(g);


	glutMainLoop();

	if (vehicle != NULL) {
		delete vehicle;
	}

	return 0;
}

void drawGoals()
{
	for (int i = 0; i < goals.size(); ++i) {
		GoalState goal = goals[i];

		glPushMatrix();
		glTranslatef(goal.x, 0, goal.z);
		glRotated(90, -1, 0, 0);
		static GLUquadric * quad = gluNewQuadric();

		// make first goal purple
		if (i == 0)
			glColor3f(1, .3, 1);
		else
			glColor3f(1, 1, 1);

		gluCylinder(quad, .5, .5, 10, 5, 1);
		glPopMatrix();
	}
}

void display() {
	frameCounter++;
	// -------------------------------------------------------------------------
	//  This method is the main draw routine. 
	// -------------------------------------------------------------------------

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(Camera::get()->isPursuitMode() && vehicle != NULL) {
		double x = vehicle->getX(), y = vehicle->getY(), z = vehicle->getZ();
		double dx = cos(vehicle->getRotation() * 3.141592765 / 180.0);
		double dy = sin(vehicle->getRotation() * 3.141592765 / 180.0);
		Camera::get()->setDestPos(x + (-20 * dx), y + 15, z + (-20 * dy));
		Camera::get()->setDestDir(dx, -0.25, dy);
	}
	Camera::get()->updateLocation();
	Camera::get()->setLookAt();

	Ground::draw();
	
    // draw test shapes
//    trap.setPosition(frameCounter, 0, 0);
//    trap.setRotation(frameCounter);
//    trap.draw();
//    tri.draw();
//    cyl.draw();
    
	// draw other vehicles
	for(std::map<int, Vehicle *>::iterator iter = otherVehicles.begin(); iter != otherVehicles.end(); ++iter) 
		iter->second->draw();

	// draw my vehicle
	if (vehicle != NULL) {
		vehicle->draw();

	}

	// draw obstacles
	ObstacleManager::get()->drawAll();

	// draw goals
	drawGoals();

	// draw HUD
	HUD::Draw();

	glutSwapBuffers();
};

void reshape(int width, int height) {

	Camera::get()->setWindowDimensions(width, height);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
};

void remoteDriver(Vehicle * vehicle, double x, double z, double r, double speed_, double steering_)
{
	// filter 
	const double kSmoothingFactor = 0.5;
	vehicle->setX(vehicle->getX() * kSmoothingFactor + x * (1 - kSmoothingFactor));
	vehicle->setZ(vehicle->getZ() * kSmoothingFactor + z * (1 - kSmoothingFactor));

	vehicle->setRotation(r);
	vehicle->steering = steering_;
	vehicle->speed = speed_;
}

double getTime()
{
#if defined(WIN32)
	LARGE_INTEGER freqli;
	LARGE_INTEGER li;
	if(QueryPerformanceCounter(&li) && QueryPerformanceFrequency(&freqli)) {
		return double(li.QuadPart) / double(freqli.QuadPart);
	}
	else {
		static ULONGLONG start = GetTickCount64();
		return (GetTickCount64() - start) / 1000.0;
	}
#else
	struct timeval t;
	gettimeofday(&t, NULL);
	return t.tv_sec + (t.tv_usec / 1000000.0);
#endif
}

void idle() {

	if (KeyManager::get()->isAsciiKeyPressed('a')) {
		Camera::get()->strafeLeft();
	}

	if (KeyManager::get()->isAsciiKeyPressed('c')) {
		Camera::get()->strafeDown();
	}

	if (KeyManager::get()->isAsciiKeyPressed('d')) {
		Camera::get()->strafeRight();
	}

	if (KeyManager::get()->isAsciiKeyPressed('s')) {
		Camera::get()->moveBackward();
	}

	if (KeyManager::get()->isAsciiKeyPressed('w')) {
		Camera::get()->moveForward();
	}

	if (KeyManager::get()->isAsciiKeyPressed(' ')) {
		Camera::get()->strafeUp();
	}

	speed = 0;
	steering = 0;

	if (KeyManager::get()->isSpecialKeyPressed(GLUT_KEY_LEFT)) {
		steering = Vehicle::MAX_LEFT_STEERING_DEGS * -1;   
	}

	if (KeyManager::get()->isSpecialKeyPressed(GLUT_KEY_RIGHT)) {
		steering = Vehicle::MAX_RIGHT_STEERING_DEGS * -1;
	}

	if (KeyManager::get()->isSpecialKeyPressed(GLUT_KEY_UP)) {
		speed = Vehicle::MAX_FORWARD_SPEED_MPS;
	}

	if (KeyManager::get()->isSpecialKeyPressed(GLUT_KEY_DOWN)) {
		speed = Vehicle::MAX_BACKWARD_SPEED_MPS;
	}

	// attempt to do data communications every 4 frames if we've created a local vehicle
	if(frameCounter % 4 == 0 && vehicle != NULL) {

		// if not connected, attempt to connect every 2 seconds
		if(!RemoteDataManager::IsConnected()) {
			if(frameCounter % 120 == 0) {
		
				// erase other vehicles
				for(std::map<int, Vehicle*>::iterator iter = otherVehicles.begin(); iter  != otherVehicles.end(); ++iter) {
					delete iter->second;
				}
				otherVehicles.clear();

				// uncomment this line to connect to the robotics server.
				RemoteDataManager::Connect("www.robotics.unsw.edu.au","18081");

				// on connect, let's tell the server what we look like
				if (RemoteDataManager::IsConnected()) {
					ObstacleManager::get()->removeAll();

					VehicleModel vm;
					vm.remoteID = 0;


                    //position for vehicle body
                    //body = new RectangularPrism(0, 1, 0, 6, 2, 4);
                    struct ShapeInit* body = new struct ShapeInit;
                    float color[3] = {1.f, 0.19f, 0.19f};
                    Convert::convertRect(body, 0, 1, 0, 6, 2, 4, color);
                    vm.shapes.push_back(*body);
                    
                    //window stuff
                    float v1[] = {1.5f, 2.f, -2.f};
                    float v2[] = {0.f, 3.5f, -2.f};
                    float v3[] = {-2.f, 3.5f, -2.f};
                    float v4[] = {-3.f, 2.f, -2.f};
                    color[0] = 0.18f; color[1] = 0.56f; color[2] = 1.f;
                    //0.18f, 0.56f, 1.f
                    struct ShapeInit* window = new struct ShapeInit;
                    Convert::convertTrap(window, v1, v2, v3, v4, 4, color);
                    vm.shapes.push_back(*window);

                    //position for front diffuser
                    float w1[] = {3.f, 0.f, -2.f};
                    float w2[] = {3.f, 1.f, -2.f};
                    float w3[] = {5.f, 0.f, -2.f};
                    //frontdiffuser = new TriangularPrism(w1, w2, w3, 4);
                    color[0] = 1.0f; color[1] = 0.19f; color[2] = 0.19f;
                    struct ShapeInit* diffuser = new struct ShapeInit;
                    Convert::convertTri(diffuser, w1, w2, w3, 4, color);
                    vm.shapes.push_back(*diffuser);
                    
                    //LRrim = new RectangularPrism(-3.0, 0.5, -2.25, 0.5, 0.5, 0.6);
                    color[0] = 0; color[1] = 0; color[2] = 1;
                    struct ShapeInit* LRrim = new struct ShapeInit;
                    Convert::convertRect(LRrim, -3, 0.5, -2.25, 0.5, 0.5, 0.6, color);
                    vm.shapes.push_back(*LRrim);
                    
                    //RRrim = new RectangularPrism(-3.0, 0.5, 2.25, 0.5, 0.5, 0.6);
                    //color[0] = 1.0f; color[1] = 0.19f; color[2] = 0.19f;
                    struct ShapeInit* RRrim = new struct ShapeInit;
                    Convert::convertRect(RRrim, -3, 0.5, 2.25, 0.5, 0.5, 0.6, color);
                    vm.shapes.push_back(*RRrim);
                    
                    //LFrim = new RectangularPrism(3.0, 0.25, -2.25, 0.5, 0.5, 0.6);
                    //color[0] = 1.0f; color[1] = 0.19f; color[2] = 0.19f;
                    struct ShapeInit* LFrim = new struct ShapeInit;
                    Convert::convertRect(LFrim, 3, 0.25, -2.25, 0.5, 0.5, 0.6, color);
                    vm.shapes.push_back(*LFrim);
                    
                    //RFrim = new RectangularPrism(3.0, 0.25, 2.25, 0.5, 0.5, 0.6);
                    //color[0] = 1.0f; color[1] = 0.19f; color[2] = 0.19f;
                    struct ShapeInit* RFrim = new struct ShapeInit;
                    Convert::convertRect(RFrim, 3, 0.25, 2.25, 0.5, 0.5, 0.6, color);
                    vm.shapes.push_back(*RFrim);
                    
                    
                    //position for wheels
                    //LRwheel = new Cylinder(-3.0, 0.5, -2.25, 1, 0.5);
                    color[0] = 1; color[1] = 1; color[2] = 1;
                    struct ShapeInit* LRwheel = new struct ShapeInit;
                    Convert::convertCyl(LRwheel, -3.0, 0.5, -2.25, 1, 0.5, color, false, true);
                    vm.shapes.push_back(*LRwheel);
                    
                    //RRwheel = new Cylinder(-3.0, 0.5, 2.25, 1, 0.5);
                    //color[0] = 1.0f; color[1] = 0.19f; color[2] = 0.19f;
                    struct ShapeInit* RRwheel = new struct ShapeInit;
                    Convert::convertCyl(RRwheel, -3.0, 0.5, 2.25, 1, 0.5, color, false, true);
                    vm.shapes.push_back(*RRwheel);
                    
                    //LFwheel = new Cylinder(3.0, 0.25, -2.25, 0.75, 0.5);
                    //color[0] = 1.0f; color[1] = 0.19f; color[2] = 0.19f;
                    struct ShapeInit* LFwheel = new struct ShapeInit;
                    Convert::convertCyl(LFwheel, 3.0, 0.25, -2.25, 0.75, 0.5, color, true, true);
                    vm.shapes.push_back(*LFwheel);
                    
                    //RFwheel = new Cylinder(3.0, 0.25, 2.25, 0.75, 0.5);
                    //color[0] = 1.0f; color[1] = 0.19f; color[2] = 0.19f;
                    struct ShapeInit* RFwheel = new struct ShapeInit;
                    Convert::convertCyl(RFwheel, 3.0, 0.25, 2.25, 0.75, 0.5, color, true, true);
                    vm.shapes.push_back(*RFwheel);
                    

					RemoteDataManager::Write(GetVehicleModelStr(vm));
				}
			}
		}

		// if we're connected, send our vehicle state to the server
		if (RemoteDataManager::IsConnected()) {
			VehicleState vs;
			vs.remoteID = 0;
			vs.x = vehicle->getX();
			vs.z = vehicle->getZ();
			vs.rotation = vehicle->getRotation();
			vs.speed = vehicle->getSpeed();
			vs.steering = vehicle->getSteering();
			RemoteDataManager::Write(GetVehicleStateStr(vs));
		}

		// if we're still connected, receive and handle response messages from the server
		if (RemoteDataManager::IsConnected()) {
			std::vector<RemoteMessage> msgs = RemoteDataManager::Read();
			for(unsigned int i = 0; i < msgs.size(); i++ ) {

				RemoteMessage msg = msgs[i];
				//cout << msg.payload << endl;

				switch(msg.type) {
					// new models
					case 'M':
						{
							std::vector<VehicleModel> models = GetVehicleModels(msg.payload);
							for(unsigned int i = 0; i < models.size(); i++) {
								VehicleModel vm = models[i];
								
								// uncomment the line below to create remote vehicles
                                
                                // pass in the vm struct to the constructor so we make it right
								otherVehicles[vm.remoteID] = new RemoteVehicle(vm);

								//
								// more student code goes here
								//
							}
							break;
						}

					// vehicle states
					case 'S': 
						{
							std::vector<VehicleState> states = GetVehicleStates(msg.payload);
							for(unsigned int i = 0; i < states.size(); i++) {
								VehicleState vs = states[i];

								std::map<int, Vehicle*>::iterator iter = otherVehicles.find(vs.remoteID);
								if(iter != otherVehicles.end()) {
									Vehicle * veh = iter->second;
									remoteDriver(veh, vs.x, vs.z, vs.rotation, vs.speed, vs.steering);
								}
							}
							break;
						}

					// goal state
					case 'G':
						{
							goals = GetGoals(msg.payload);
							break;
						}

					// obstacle state
					case 'O':
						{
							std::vector<ObstacleState> obs = GetObstacles(msg.payload);
							for(unsigned int i = 0; i < obs.size(); i++) {
								Obstacle o(obs[i].x, obs[i].z, obs[i].radius);
								ObstacleManager::get()->addObstacle(o);
							}
							break;
						}

					// disconnect list
					case 'D':
						{
							std::vector<int> disconnectedIDs = GetVehicleDisconnects(msg.payload);
							for(unsigned int i = 0; i < disconnectedIDs.size(); i++) {
								int id = disconnectedIDs[i];
								std::map<int, Vehicle*>::iterator iter = otherVehicles.find(id);
								if(iter != otherVehicles.end()) {
									delete iter->second;
									otherVehicles.erase(iter);
								}
							}
							break;
						}

					// error message
					case 'E':
						{
							cerr << "Server error: " << msg.payload << endl;
							break;
						}

				}
			} 
		}
	}


	const float sleep_time_between_frames_in_seconds = 0.025;

	static double previousTime = getTime();
	const double currTime = getTime();
	const double elapsedTime = currTime - previousTime;
	previousTime = currTime;

	// do a simulation step
	if (vehicle != NULL) {
        if (!isChasing) {
            vehicle->update(speed, steering, elapsedTime);
        } else {
            // we want to chase vehicle ID 1
            for(std::map<int, Vehicle*>::iterator it = otherVehicles.begin(); it  != otherVehicles.end(); ++it) {
                
                if (it->second->getID() == 1) {
                    // this is a vector from our local vehicle pointing to the vehicle we want to chase
                    double chaseVectorX = it->second->getX() - vehicle->getX();
                    double chaseVectorZ = it->second->getZ() - vehicle->getZ();
                    // angle between our local vehicle and the one we want to chase
                    cout << "X: " << chaseVectorX << " Z: " << chaseVectorZ << " Theta: " << 180*atan2(chaseVectorZ, chaseVectorX)/M_PI << " Rot: " << vehicle->getRotation() << endl;
                    double localRotation = vehicle->getRotation();
                    if (localRotation > 180) {
                        localRotation = localRotation - 360;
                    }
                    double theta = localRotation - 180*atan2(chaseVectorZ, chaseVectorX)/M_PI;
                    //cout << 180*atan2(chaseVectorZ, chaseVectorX)/M_PI << endl;
                    if (abs(theta) >= Vehicle::MAX_LEFT_STEERING_DEGS) {
                        // set theta to be at most the max steering angle
                        theta = Vehicle::MAX_LEFT_STEERING_DEGS*theta/abs(theta);
                    }
                    double dist = sqrt(pow(chaseVectorX, 2) + pow(chaseVectorZ, 2));
                    if (dist >= Vehicle::MAX_FORWARD_SPEED_MPS) {
                        dist = Vehicle::MAX_FORWARD_SPEED_MPS;
                    }
                    //cout << "X: " << chaseVectorX << " Z: " << chaseVectorZ << " Theta: " << theta << " dist: " << dist << endl;
                    
                    vehicle->update(dist, -theta, elapsedTime);
                    
                } else {
                    // we couldn't find vehicle with ID 1
                    //isChasing = false;
                    //vehicle->update(speed, steering, elapsedTime);
                }
            }
            
        }
	}
	for(std::map<int, Vehicle*>::iterator iter = otherVehicles.begin(); iter  != otherVehicles.end(); ++iter) {
		iter->second->update(elapsedTime);
	}

	display();

#ifdef _WIN32 
	Sleep(sleep_time_between_frames_in_seconds * 1000);
#else
	usleep(sleep_time_between_frames_in_seconds * 1e6);
#endif
};

void keydown(unsigned char key, int x, int y) {

	// keys that will be held down for extended periods of time will be handled
	//   in the idle function
	KeyManager::get()->asciiKeyPressed(key);

	// keys that react ocne when pressed rather than need to be held down
	//   can be handles normally, like this...
	switch (key) {
	case 27: // ESC key
		exit(0);
		break;      
	case '0':
		Camera::get()->jumpToOrigin();
		break;
	case 'p':
		Camera::get()->togglePursuitMode();
		break;
    case 'l':
        // chase vehicle with ID 1
        // toggle isChasing
        isChasing = !isChasing;
        break;
	}

};

void keyup(unsigned char key, int x, int y) {
	KeyManager::get()->asciiKeyReleased(key);
};

void special_keydown(int keycode, int x, int y) {

	KeyManager::get()->specialKeyPressed(keycode);

};

void special_keyup(int keycode, int x, int y) {  
	KeyManager::get()->specialKeyReleased(keycode);  
};

void mouse(int button, int state, int x, int y) {

};

void dragged(int x, int y) {

	if (prev_mouse_x >= 0) {

		int dx = x - prev_mouse_x;
		int dy = y - prev_mouse_y;

		Camera::get()->mouseRotateCamera(dx, dy);
	}

	prev_mouse_x = x;
	prev_mouse_y = y;
};

void motion(int x, int y) {

	prev_mouse_x = x;
	prev_mouse_y = y;
};


