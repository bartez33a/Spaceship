#pragma once
#include "Cube.h"
#include <math.h>

// rocket class
class Rocket :
	public Cube
{
public:
	// constructor
	Rocket(Shader *s, float x, float y, float z, glm::vec3 dir); //constructor
	// constructor for rockets with texture
	Rocket(Shader *s, float x, float y, float z, int rep, int texNo, glm::vec3 dir ); //constructor
	// copy constructor
	Rocket(const Rocket & rocket);
	//move constructor
	Rocket(const Rocket && rocket); 
	// default constructor
	Rocket() = delete;
	// destructor
	~Rocket(); 
	// assignment operator
	Rocket& operator=(const Rocket &rocket);
	// this function returns distance covered from init position
	float checkDistance();
	// function for moving object
	void move(double deltaTime); 
private:
	//direction of movement
	glm::vec3 direction;
	//speed of rocket
	float speed;
	//distance from starting point
	float distance;
};

