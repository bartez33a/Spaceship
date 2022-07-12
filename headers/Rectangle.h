#pragma once

#include "../headers/VAO.h"
#include "../headers/VBO.h"
#include "../headers/EBO.h"

class Rectangle
{
	float x_, y_, z_;
	float w_, h_;
	float R_, G_, B_;
	VAO vao;
	VBO vbo;
	EBO ebo;


public:
	Rectangle(float x, float y, float z, float w, float h, float R = 1.0f, float G = 1.0f, float B = 1.0f);
	~Rectangle();
	void draw();

};

