#pragma once

#include "../headers/VAO.h"
#include "../headers/VBO.h"

#include "Object.h"
#include "Shape.h"
#include <iostream>

class Cube : public Shape
{
protected:
	//cube has initial position from Class Shape -> float x_, y_, z_
	// modelMatrix to transform cube vertices
	glm::mat4 modelMatrix; //model matrix to move body
	// actual position of cube
	glm::vec3 position; //actual position of body
	float w_, h_, l_;
public:
	Cube(Shader *shader, float x, float y, float z, float w, float h, float l, float R, float G, float B );
	Cube(Shader *shader, float x, float y, float z, float w, float h, float l, int rep, float R, float G, float B );
	~Cube();
	void draw();
	void draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
	glm::vec3 getPosition() const;
	glm::vec3 getDimensions() const;
	friend std::ostream& operator << (std::ostream& str, Cube const &c);
	glm::mat4 getModelMatrix() const;
};

