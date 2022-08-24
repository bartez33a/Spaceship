#include "../headers/VAO.h"

// vertex array object constructor
// generates VAO
VAO::VAO()
{
	//create buffer
	glGenVertexArrays(1, &ID);	
}

// destructor 
// deletes VAO
VAO::~VAO()
{
	//delete buffer
	glDeleteBuffers(1, &ID);
}

// function for binding VAO
void VAO::BindBuffer()
{
	// Bind VAO
	glBindVertexArray(ID);
}

// function for unbinding VAO
void VAO::UnbindBuffer()
{
	//unbind VAO
	glBindVertexArray(0);
}

// function for setting VAO attributes
// location - location of specified attribute
// attribSize - size of single vertex attribute (e.g. location, color) in bytes 
// stride - spacing between start of this specified attribute of vertex in bytes,
//			only number of float variables, there is multiplication by sizeof(float)
//offset - offset of this specified attribute in bytes
//			only number of float variables, there is multiplication by sizeof(float)
void VAO::SetAttribPointer(VBO &vbo, int location, int attribSize, int stride, int offset)
{
	//Bind VBO to set attributes
	vbo.BindBuffer();

	//stride - spacing between start of this specified attribute of vertex in bytes
	//offset - offset of this specified attribute in bytes
	glVertexAttribPointer(location, attribSize, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset*sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(location);
	//now we can unbind VBO while VAO is still binded!
	vbo.UnbindBuffer();
}