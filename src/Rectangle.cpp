#include "../headers/Rectangle.h"

// constructor for rectangle without texture
Rectangle::Rectangle(float x, float y, float z, float w, float h, float R, float G, float B): x_(x), y_(y), z_(z), w_(w), h_(h),
R_(R), G_(G), B_(B)
{

	//for EBO and glDrawElements
	float vertices_rect[] = {
		x,			y,			0,		R_, G_, B_,
		x + w,		y,			0,		R_, G_, B_,
		x + w,		y + h,		0,		R_, G_, B_,
		x,			y + h,		0,		R_, G_, B_
	};

	GLuint indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	//it's for glDrawArrays, we have to change VBO data!
	/*
	float vertices[] = {
		x,			y,			z_,		R_, G_, B_,
		x + w,		y,			z_,		R_, G_, B_,
		x + w,		y + h,		z_,		R_, G_, B_,
		x + w,		y + h,		z_,		R_, G_, B_,
		x,			y + h,		z_,		R_, G_, B_,
		x,			y,			z_,		R_, G_, B_
	};
	*/


	vbo.LoadBufferData(vertices_rect, sizeof(vertices_rect));
	vao.BindBuffer();
	ebo.LoadBufferData(indices, sizeof(indices));
	vao.SetAttribPointer(vbo, 0, 3, 6, 0); //coordinates
	vao.SetAttribPointer(vbo, 1, 3, 6, 3); //color
	vao.UnbindBuffer();
	ebo.UnbindBuffer();
}


Rectangle::~Rectangle()
{
	vao.UnbindBuffer();
	vbo.UnbindBuffer();
}

// draw rectangle
void Rectangle::draw()
{
	vao.BindBuffer();
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

