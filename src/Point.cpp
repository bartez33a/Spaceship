#include "..\headers\Point.h"

Point::Point(Shader * shader, float x, float y, float z, float size): Shape(shader, x, y, z), m_size { size }
{
	// point consist of only one vertex
	float vertex[] = {
	x,	y,	z,	R_,	G_,	B_
	};
	
	vbo.LoadBufferData(vertex, sizeof(vertex));
	vao.BindBuffer();
	vao.SetAttribPointer(vbo, 0, 3, 6, 0); //coordinates
	vao.SetAttribPointer(vbo, 1, 3, 6, 3); //color
	vao.UnbindBuffer();
	ebo.UnbindBuffer();
}

Point::~Point()
{
	vao.UnbindBuffer();
	vbo.UnbindBuffer();
	ebo.UnbindBuffer();
}

void Point::draw()
{
	// Bind VAO
	vao.BindBuffer();
	// use shader program
	shader_->use();
	// change size of point
	glPointSize(m_size);
	// draw point
	glDrawArrays(GL_POINTS, 0, 1);
}
