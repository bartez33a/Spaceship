#include "../headers/VAO.h"


VAO::VAO()
{
	//create buffer
	glGenVertexArrays(1, &ID);	
}

VAO::~VAO()
{
	//delete buffer
	glDeleteBuffers(1, &ID);
}

void VAO::BindBuffer()
{
	// Bind VAO
	glBindVertexArray(ID);
}

void VAO::UnbindBuffer()
{
	//unbind VAO
	glBindVertexArray(0);
}

//stride -> only number of float variables, there is multiplication by sizeof(float)
//offset -> only number of float variables, ...
void VAO::SetAttribPointer(VBO &vbo, int location, int attribSize, int stride, int offset)
{
	//Bind VBO to set attributes
	vbo.BindBuffer();

	//attribSize - size of single vertex attribute (e.g. location, color) in bytes
	//stride - spacing between start of this specified attribute of vertex in bytes
	//offset - offset of this specified attribute in bytes
	glVertexAttribPointer(location, attribSize, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset*sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(location);
	//now we can unbind VBO while VAO is still binded!
	vbo.UnbindBuffer();
}