#include "../headers/VBO.h"

//default constructor only generates buffer
VBO::VBO()
{
	//create VBO
	glGenBuffers(1, &ID);
}

//constructor creates VBO and load vertices data of size vertices_size to this buffer
//vertices -> addres to the beggining of data (pointer)
//vertices size -> total size of data in bytes
VBO::VBO(float *vertices, unsigned int vertices_size)
{
	//create VBO
	glGenBuffers(1, &ID);
	//Bind buffer
	this->BindBuffer();
	//and load data to this buffer
	glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);
	this->UnbindBuffer();
}

// destructor deletes buffer
VBO::~VBO()
{
	//delete buffer
	glDeleteBuffers(1, &ID);
}

// function to bind VBO
void VBO::BindBuffer()
{
	//Bind buffer 
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

// function to unbind VBO
void VBO::UnbindBuffer()
{	
	//unbind buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// function for loading data to VBO
//vertices -> addres to the beggining of data (pointer)
//vertices size -> total size of data in bytes
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