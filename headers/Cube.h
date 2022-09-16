#pragma once

#include "../headers/VAO.h"
#include "../headers/VBO.h"

#include "Shape.h"
#include <iostream>

class Cube : public Shape
{
public:
	// constructor for cube without texture
	Cube(Shader *shader, float x, float y, float z, float w, float h, float l, float R, float G, float B );
	// constructor for cube with texture
	Cube(Shader *shader, float x, float y, float z, float w, float h, float l, int rep, float R, float G, float B );
	//default constructor
	Cube() = delete;
	// destructor
	~Cube();
	//function for drawing object
	void draw();
	//function for drawing objects with texture
	void draw_tex();
	//getters
	glm::vec3 getPosition() const;
	glm::vec3 getDimensions() const;
	glm::mat4 getModelMatrix() const;
	// operator for printing object
	friend std::ostream& operator << (std::ostream& str, Cube const &c);
protected:
	//cube has initial position from Class Shape -> float x_, y_, z_
	glm::mat4 modelMatrix; //model matrix to move body
	glm::vec3 position; //actual position of body
	float w_, h_, l_; //width, height, length of body
};

