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
	static int counter;
	//Cube c; //body
	//direction for meteor movement
	glm::vec3 direction;
	//speed for meteor movement
	float speed;

public:
	Meteor(Shader *s, float x, float y, float z, float r, float R = 1.0f, float G = 1.0f, float B = 1.0f);
	Meteor(Shader *s, float x, float y, float z, float r, int rep, int tex_no, float R = 1.0f, float G = 1.0f, float B = 1.0f);
	Meteor(const Meteor & m);
	Meteor(const Meteor & m, int rep, int tex_no); //with texture
	~Meteor();

	void move(double deltaTime);
	glm::vec3 getPosition() const;
	float checkDistance(); //check covered distance from init point.
	
};

