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
	unsigned int ID;
	int modelLoc; 
	int viewLoc; 
	int projectionLoc;
	glm::mat4 m_model;
	glm::mat4 m_view;
	glm::mat4 m_projection;

public:
	Shader();
	Shader(const char* vs_file, const char* fs_file);
	~Shader();
	void use(); 
	unsigned int get_ID() const;
	void createShader(const char* vs_file, const char* fs_file);
	void setUniformMatrix(int location, glm::mat4 matrix);
	void setUniformfloat(std::string name, float value);
	void setUniformInt(std::string name, int value);
	int getModelMatrixLocation() const;
	int getViewMatrixLocation() const;
	int getProjectionMatrixLocation() const;
	void updateMatrices(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
};

