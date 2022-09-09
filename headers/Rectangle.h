#pragma once

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Shader.h"

namespace shapes{
	class Rectangle
	{
	public:
		// constructor
		Rectangle(Shader& shader, float x, float y, float z, float w, float h, float R = 1.0f, float G = 1.0f, float B = 1.0f);
		// default constructor
		Rectangle() = delete;
		// destructor
		~Rectangle();
		// drawing function
		void draw();
		void drawLines();
		//update vertices
		void updateDimensions(float x, float y, float w, float h);
		void setShader(Shader& Shader);
	private:
		//Shader for drawing
		Shader m_Shader;
		// coordinates of left, bottom corner
		float x_, y_, z_;
		// width and height
		float w_, h_;
		// color of vertex
		float R_, G_, B_;
		// openGL buffers
		VAO vao;
		VBO vbo;
		EBO ebo;
	};
}

