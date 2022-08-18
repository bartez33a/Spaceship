#include "..\headers\Point.h"

//int Point::noOfPoints = 0;

Point::Point(Shader * shader, float x, float y, float z, float size): Shape(shader, x, y, z), m_size { size }
{
	//noOfPoints++;
	//std::cout << "noOfPoints = " << noOfPoints << '\n';
	float vertices[] = {
	x,			y,			z, R_, G_, B_
	};


	vbo.LoadBufferData(vertices, sizeof(vertices));
	vao.BindBuffer();
	vao.SetAttribPointer(vbo, 0, 3, 6, 0); //coordinates
	vao.SetAttribPointer(vbo, 1, 3, 6, 3); //color
	vao.UnbindBuffer();
	ebo.UnbindBuffer();

}

Point::~Point()
{
	//std::cout << "DESTRUCTOR!!! noOfPoints = " << noOfPoints << '\n';
}

void Point::draw()
{
	vao.BindBuffer();
	shader_->use();
	glPointSize(m_size);
	glDrawArrays(GL_POINTS, 0, 1);
}
