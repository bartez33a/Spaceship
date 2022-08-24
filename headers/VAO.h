#pragma once

#include <glad\glad.h>
#include "../headers/VBO.h"

class VAO
{
public:
	// constructor for vertex array object
	VAO();
	// destructor
	~VAO();
	// bind VAO
	void BindBuffer();
	// unbind VAO
	void UnbindBuffer();
	// set attributes for VAO
	void SetAttribPointer(VBO &vbo, int location, int attribSize, int stride, int offset);
private:
	// VAO ID
	GLuint ID;
};

