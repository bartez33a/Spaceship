#pragma once
#include "Cube.h"
#include "Sphere.h"
#include "Shader.h"
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <cstdlib>
#include <time.h>

class Meteor: public Sphere
{
public:
	// constructor of meteor without texture
	Meteor(Shader *s, float x, float y, float z, float r, float R = 1.0f, float G = 1.0f, float B = 1.0f);
	// constructor of meteor with texture
	Meteor(Shader *s, float x, float y, float z, float r, int rep, int tex_no, float R = 1.0f, float G = 1.0f, float B = 1.0f);
	// copy constructor of meteor without texture
	Meteor(const Meteor & m);
	// copy constructor of meteor with texture
	Meteor(const Meteor & m, int rep, int tex_no); //with texture
	// destructor of meteor
	~Meteor();

	void move(double deltaTime);
	//get actual position of meteor -> position derived from Sphere
	glm::vec3 getPosition() const; 
	float checkDistance(); //check covered distance from init point.
private:
	/// variables
	//direction vector for meteor movement
	glm::vec3 direction;
	//speed of meteor movement
	float speed;
};

