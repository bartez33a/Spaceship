#pragma once

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Shader.h"

//base class for all shapes e.g. cube, sphere, rectangle
class Shape
{
public:
	//constructor
	Shape(Shader *shader, float x, float y, float z, float R = 1.0f, float G = 1.0f, float B = 1.0f);
	// default constructor
	Shape() = delete;
	// destructor
	~Shape();
protected: 
	// position of origin
	float x_, y_, z_;
	// color of shape
	float R_, G_, B_;
	//initial position of body
	glm::vec3 init_pos;
	//openGL buffers
	VAO vao;
	VBO vbo;
	EBO ebo;
	// shader object
	Shader *shader_;
	// texture number
	int m_tex_no;
};

