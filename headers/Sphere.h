#pragma once
#include "Shape.h"
#include "Sphere.h"

#include <vector>
#include <math.h>

class Sphere : public Shape
{
protected:
	float radius;
	int vertices_number;
	int indices_number;
	glm::mat4 modelMatrix; //model matrix to move body
	glm::vec3 position; //actual position of body
public:
	/*from Shape*/
		/*
		//position of origin
		float x_, y_, z_;
		//color of shape
		float R_, G_, B_;
		//initial position of body
		glm::vec3 init_pos;
		//openGL buffers
		VAO vao;
		VBO vbo;
		EBO ebo;
		Shader *shader_;
		*/

	Sphere(Shader *shader, float x, float y, float z, float r, float R = 1.0f, float G = 1.0f, float B = 1.0f);
	Sphere(Shader *shader, float x, float y, float z, float r, int rep, int tex_no, float R, float G, float B);
	~Sphere();
	void draw();
	void draw_tex();
	float getRadius() const;
	glm::vec3 getPosition() const;
};

