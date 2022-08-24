#pragma once
#include "Shape.h"
#include "Sphere.h"

#include <vector>
#include <math.h>

class Sphere : public Shape
{
public:
	// constructor for sphere without texture
	Sphere(Shader *shader, float x, float y, float z, float r, float R = 1.0f, float G = 1.0f, float B = 1.0f);
	// constructor for sphere with texture
	Sphere(Shader *shader, float x, float y, float z, float r, int rep, int tex_no, float R, float G, float B);
	// default constructor
	Sphere() = delete;
	// destructor
	~Sphere();
	// function for drawing object without selecting number of texture
	void draw();
	// function for drawing object with selecting number of texture from m_tex_no variable from Shape class
	void draw_tex();
	// get radius of sphere
	float getRadius() const;
	// get current position of sphere
	glm::vec3 getPosition() const;
protected:
	// radius of sphere
	float radius;
	// number of vertices
	int vertices_number;
	// number of indices
	int indices_number;
	//model matrix to move body
	glm::mat4 modelMatrix;
	//actual position of body
	glm::vec3 position; 
};

