#pragma once

#include <glad\glad.h>
#include <iostream>

class VBO
{
	GLuint ID;
public:
	VBO();
	//Constructor for Vertex buffer object and load data
	VBO(float *vertices, unsigned int vertices_size);
	//Destructor deletes buffer
	~VBO();
	void BindBuffer();
	void UnbindBuffer();
	void LoadBufferData(float *vertices, unsigned int vertices_size);
};

