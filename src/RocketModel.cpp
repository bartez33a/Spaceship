#include "..\headers\RocketModel.h"

//constructor of rocket model object
// path -> path to model file
// x,y,z -> coordinates of object
// dir -> camera direction
// inverse_x,y,z -> if true: inverse coordinates of indicated axis of model while loading object
RocketModel::RocketModel(std::string path, float x, float y, float z, glm::vec3 cameraAngles, bool inverse_x, bool inverse_y, bool inverse_z):
position{ x, y, z },
init_position{ x, y, z },
distance{0.0f},
speed{5.0f},
model(path, inverse_x, inverse_y, inverse_z)
{
	// initialize model matrix
	model_Matrix = glm::mat4(1.0f);
	// first set position of rocket
	model_Matrix = glm::translate(model_Matrix, glm::vec3(x, y, z));
	//then rotate object depenging on camera's direction (based on pitch and yaw angles)
	//yaw (ruch poziomy
	// yaw has offset (-90.0f so we add 90.0f)
	model_Matrix = glm::rotate(model_Matrix, glm::radians(cameraAngles.x + 90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	// pitch (ruch pionowy)
	model_Matrix = glm::rotate(model_Matrix, glm::radians(cameraAngles.y), glm::vec3(1.0f, 0.0f, 0.0f));
	
	// model matrix is rotated so we want to move in oposite direction of axis Z
	direction = glm::vec3(0.0f, 0.0f, -1.0f);
	// save path
	this->path = path;
}

// copy constructor for rocket model so we dont have to create new model each time
// x,y,z -> new position of rocket model
// cameraAngles -> vector of camera angles for rotating model
RocketModel::RocketModel(const RocketModel& rocketModel, 
	float x, float y, float z, glm::vec3 cameraAngles): 
	position{ x, y, z },
	init_position{ x, y, z },
	distance{ 0.0f },
	speed{ 5.0f },
	model{rocketModel.model}
{
	// model matrix
	model_Matrix = glm::mat4(1.0f);	
	// first set position of rocket
	model_Matrix = glm::translate(model_Matrix, glm::vec3(x, y, z));
	//then rotate object depenging on camera's direction (based on pitch and yaw angles)
	//yaw (ruch poziomy
	// yaw has offset (-90.0f so we add 90.0f)
	model_Matrix = glm::rotate(model_Matrix, glm::radians(cameraAngles.x + 90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	// pitch (ruch pionowy)
	model_Matrix = glm::rotate(model_Matrix, glm::radians(cameraAngles.y), glm::vec3(1.0f, 0.0f, 0.0f));
	
	// model matrix is rotated so we want to move in oposite direction of axis Z
	direction = glm::vec3(0.0f, 0.0f, -1.0f);
	
	// copy path
	this->path = path;
}

// function for checking covered distance
float RocketModel::checkDistance()
{
	distance = sqrt(pow((position.x - init_position.x), 2.0f) + pow((position.y - init_position.y), 2.0f) +
		pow((position.z - init_position.z), 2.0f));
	return distance;
}

// function for moving object
void RocketModel::move(double deltaTime)
{
	float speed_factor = speed * deltaTime;
	//visualise position.
	position += direction * speed_factor;
	// move model in vertex shader
	direction = glm::vec3(0.0f, 0.0f, -1.0f);
	model_Matrix = glm::translate(model_Matrix, direction * speed_factor);
}

// function for drawing rocket model
void RocketModel::Draw(Shader& shader, float scale)
{
	// we have to update model matrix in vertex shader
	shader.setUniformMatrix(shader.getModelMatrixLocation(), model_Matrix);
	// then we call Draw function of model
	model.Draw(shader, scale);
}

// this function returns bounding box from model
glm::mat3x2 RocketModel::getBoundingBox() const
{
	return model.getBoundingBox();
}

// function for getting position (glm::vec3) of object
glm::vec3 RocketModel::getPosition() const
{
	return position;
}
