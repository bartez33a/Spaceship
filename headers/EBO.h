#pragma once

#include <glad\glad.h>

class EBO
{
	GLuint ID;
public:
	EBO();
	~EBO();
	void BindBuffer();
	void UnbindBuffer();
	void LoadBufferData(GLuint *indices, unsigned int indices_size);
};

