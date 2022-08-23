#pragma once

#include <glad\glad.h>

// OpenGL - element buffer object class
class EBO
{
	// ID of EBO
	GLuint ID;
public:
	EBO();
	~EBO();
	void BindBuffer();
	void UnbindBuffer();
	void LoadBufferData(GLuint *indices, unsigned int indices_size);
};

