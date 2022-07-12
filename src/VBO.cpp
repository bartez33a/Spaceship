#include "../headers/VBO.h"

//default constructor only generates buffer
VBO::VBO()
{
	//create VBO
	glGenBuffers(1, &ID);
}

//constructor creates VBO and load vertices data of size vertices_size to this buffer
VBO::VBO(float *vertices, unsigned int vertices_size)
{
	//create VBO
	glGenBuffers(1, &ID);
	//Bind buffer
	this->BindBuffer();
	//and load data to this buffer
	//vertices size -> total size in bytes of data
	//vertices -> addres to the beggining of data
	glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);
	this->UnbindBuffer();
}


VBO::~VBO()
{
	//delete buffer
	glDeleteBuffers(1, &ID);
}

void VBO::BindBuffer()
{
	//Bind buffer 
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::UnbindBuffer()
{	
	//unbind buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::LoadBufferData(float * vertices, unsigned int vertices_size)
{
	//Bind buffer
	this->BindBuffer();
	//and load data to this buffer
	//vertices size -> total size in bytes of data
	//vertices -> addres to the beggining of data
	glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);
	this->UnbindBuffer();
}