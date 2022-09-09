#include "../headers/Rectangle.h"

namespace shapes {

	// constructor for rectangle without texture
	Rectangle::Rectangle(Shader& shader, float x, float y, float z, float w, float h, float R, float G, float B) : x_(x), y_(y), z_(z), w_(w), h_(h),
		R_(R), G_(G), B_(B), 
		m_Shader{shader}
	{

		//for EBO and glDrawElements
		float vertices_rect[] = {
			x,			y,			z,				R_, G_, B_,
			x + w,		y,			z,				R_, G_, B_,
			x + w,		y + h,		z,				R_, G_, B_,
			x,			y + h,		z,				R_, G_, B_
		};
		
		//indices for EBO
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
		// use shader
		m_Shader.use();
		// draw rectangle
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	// function for drawing rectangle without fill
	void Rectangle::drawLines()
	{
		// bind buffer
		vao.BindBuffer();
		//use shader
		m_Shader.use();
		// draw rectangle without fill
		glDrawElements(GL_LINE_STRIP, 6, GL_UNSIGNED_INT, 0);
	}
	
	//function for update dimensions of rectangle:
	// (x,y) - coordinates of bottom left corner of rectangle
	// w - width of rectangle
	// h - height of rectangle
	// we only update Vertex Buffer Object!
	void Rectangle::updateDimensions(float x, float y, float w, float h)
	{
		float vertices_rect[] = {
			x,			y,			z_,				R_, G_, B_,
			x + w,		y,			z_,				R_, G_, B_,
			x + w,		y + h,		z_,				R_, G_, B_,
			x,			y + h,		z_,				R_, G_, B_
		};

		//bind VBO
		vbo.BindBuffer();
		//and update data
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices_rect), vertices_rect);
		// EBO is identical
	}
} //namespace shapes