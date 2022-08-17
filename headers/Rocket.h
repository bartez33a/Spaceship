#pragma once
#include "Cube.h"
#include <math.h>

class Rocket :
	public Cube
{
	static int counter;
	//direction of movement
	glm::vec3 direction;
	//speed of rocket
	float speed;
	//distance from starting point
	float distance;
public:
	Rocket(Shader *s, float x, float y, float z, glm::vec3 dir); //constructor
	Rocket(const Rocket & rocket);
	Rocket(const Rocket && rocket); //move constructor
	~Rocket(); //destructor
	float checkDistance(); //check distance from init point
	void move(double deltaTime); //move object
	//glm::vec3 getPosition() const; //get actual position of object
	Rocket& operator=(const Rocket &rocket);
};

