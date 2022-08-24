#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <glad\glad.h> //for OpenGL functions
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	// constructor
	Shader(const char* vs_file, const char* fs_file);
	// defualt constructor
	Shader() = delete;
	//destructor
	~Shader();
	//use shader program
	void use(); 
	// get shader ID
	unsigned int get_ID() const;
	// set uniform matrix
	void setUniformMatrix(int location, glm::mat4 matrix);
	// set uniform float
	void setUniformfloat(std::string name, float value);
	// set uniform int
	void setUniformInt(std::string name, int value);
	// get model matrix location in shader program
	int getModelMatrixLocation() const;
	// get view matrix location in shader program
	int getViewMatrixLocation() const;
	// get projection matrix location in shader program
	int getProjectionMatrixLocation() const;
	// update modelm view and projection matrices in shader
	void updateMatrices(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
	// function for rotating objects
	void rotateObjects(float angle);
private:
	// shader ID
	unsigned int ID;
	// location of model matrix
	int m_modelLoc;
	// location of view matrix
	int m_viewLoc;
	// location of projection matrix
	int m_projectionLoc;
	// model matrix
	glm::mat4 m_model;
	// view matrix
	glm::mat4 m_view;
	// projection matrix
	glm::mat4 m_projection;

};

