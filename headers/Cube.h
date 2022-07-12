#pragma once

#include "../headers/VAO.h"
#include "../headers/VBO.h"

class Cube
{
	float x_, y_, z_;
	float w_, h_, l_;
	float R_, G_, B_;
	VAO vao;
	VBO vbo;
public:
	Cube(float x, float y, float z, float w, float h, float l, float R=1.0f, float G = 1.0f, float B = 1.0f);
	Cube(float x, float y, float z, float w, float h, float l, int rep, float R = 1.0f, float G = 1.0f, float B = 1.0f);
	~Cube();
	void draw();


};

