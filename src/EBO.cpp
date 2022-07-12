#include "../headers/EBO.h"


EBO::EBO()
{
	glGenBuffers(1, &ID);
}

EBO::~EBO()
{
	glDeleteBuffers(1, &ID);
}

void EBO::BindBuffer()
{
	//Bind buffer 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::UnbindBuffer()
{
	//Unbind buffer 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::LoadBufferData(GLuint* indices, unsigned int indices_size)
{
	this->BindBuffer();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);
}