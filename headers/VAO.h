#pragma once

#include <glad\glad.h>
#include "../headers/VBO.h"

class VAO
{
	GLuint ID;
public:
	VAO();
	~VAO();
	void BindBuffer();
	void UnbindBuffer();
	void SetAttribPointer(VBO &vbo, int location, int attribSize, int stride, int offset);
};

