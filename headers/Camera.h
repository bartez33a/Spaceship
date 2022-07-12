#pragma once
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <GLFW\glfw3.h>
#include <iostream>

//it's a FPS camera
class Camera
{
	glm::vec3 position; //position of camera
	glm::vec3 front; // target of camera is camera's front!
	glm::vec3 up_vector; //global coordinate system's up vector
	glm::mat4 view_matrix;
	float deltaTime;
	
	float pitch;
	float yaw;
	float roll;

	bool key_space_lastState;
	bool jump;
	float jumpTime;

public:
	//constructor 
	Camera(glm::vec3 pos, glm::vec3 dir, glm::vec3 up_vec);
	//destructor
	~Camera();
	//function that records input
	void Input(GLFWwindow *window, float deltaTime);
	//returns viewMatrix
	void mouseInput(float xoffset, float yoffset);
	glm::mat4 get_viewMatrix() const;

	void updateCameraVectors();

};

