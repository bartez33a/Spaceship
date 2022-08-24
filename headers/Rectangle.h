#pragma once

#include "../headers/VAO.h"
#include "../headers/VBO.h"
#include "../headers/EBO.h"

class Rectangle
{
public:
	// constructor
	Rectangle(float x, float y, float z, float w, float h, float R = 1.0f, float G = 1.0f, float B = 1.0f);
	// default constructor
	Rectangle() = delete;
	// destructor
	~Rectangle();
	// drawing function
	void draw();
private:
	// coordinates of left, bottom corner
	float x_, y_, z_;
	// width and height
	float w_, h_;
	// color of vertex
	float R_, G_, B_;
	// openGL buffers
	VAO vao;
	VBO vbo;
	EBO ebo;
};

