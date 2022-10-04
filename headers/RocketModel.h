#pragma once
#include "Model.h"
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/rotate_vector.hpp>

class RocketModel
{
public:
	//constructor
	RocketModel(std::string path, float x, float y, float z, glm::vec3 dir, bool inverse_x, bool inverse_y, bool inverse_z); //constructor
	//move constructor
	RocketModel(RocketModel&& rocketModel) = delete;
	//copy constructor
	RocketModel(const RocketModel& rocketModel, float x, float y, float z, glm::vec3 cameraAngles);

	// this function returns distance covered from init position
	float checkDistance();
	// function for moving object
	void move(double deltaTime);
	// function for drawing rocket model
	void Draw(Shader& shader, float scale);
	//get bounding box function
	glm::mat3x2 getBoundingBox() const;
	//get actual position of model
	glm::vec3 getPosition() const;
private:
	// model object
	Model model;
	// initial position
	const glm::vec3 init_position;
	//actual position
	glm::vec3 position;
	//direction of movement
	glm::vec3 direction;
	//speed of rocket
	double speed;
	//distance from starting point
	float distance;
	//model matrix
	glm::mat4 model_Matrix;
	// model file
	std::string path;
};

