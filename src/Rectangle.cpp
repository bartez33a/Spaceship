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

	vbo.LoadBufferData(vertices_rect, sizeof(vertices_rect));
	vao.BindBuffer();
	ebo.LoadBufferData(indices, sizeof(indices));
	vao.SetAttribPointer(vbo, 0, 3, 6, 0); //coordinates
	vao.SetAttribPointer(vbo, 1, 3, 6, 3); //color
	vao.UnbindBuffer();
	ebo.UnbindBuffer();
}

// destructor
Rectangle::~Rectangle()
{
	vao.UnbindBuffer();
	vbo.UnbindBuffer();
}

// function for drawing rectangle
// before call of this function you should make shader program active
void Rectangle::draw()
{
	// bind buffer
	vao.BindBuffer();
	// draw rectangle
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

