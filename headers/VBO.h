#pragma once

#include <glad\glad.h>
#include <iostream>


class VBO
{
public:
	//default constructor only creates vertex buffer object
	VBO();
	//constructor creates Vertex buffer object and loads vertices data of size vertices_size to this buffer
	VBO(float *vertices, unsigned int vertices_size);
	//Destructor deletes buffer
	~VBO();
	// bind buffer
	void BindBuffer();
	// unbind buffer
	void UnbindBuffer();
	// load vertices data to buffer
	void LoadBufferData(float *vertices, unsigned int vertices_size);
private:
	// VBO ID
	GLuint ID;
};

