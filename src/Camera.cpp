#include "../headers/Camera.h"



Camera::Camera(glm::vec3 pos, glm::vec3 front_, glm::vec3 up_vec): position(pos), front(front_), up_vector(up_vec)
{
	view_matrix = glm::lookAt(position, position + front, up_vector);
	yaw = -90.0f;
}


Camera::~Camera()
{
}


//input for camera movement
void Camera::Input(GLFWwindow * window, float deltaTime)
{
	float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position += front * cameraSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position -= front * cameraSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position -= glm::normalize(glm::cross(up_vector, front)) * cameraSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position += glm::normalize(glm::cross(up_vector, front)) * cameraSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (!key_space_lastState)
		{
			//positive edge!
			//hop
			jump = true;
			jumpTime = glfwGetTime();
			std::cout << "JUMP!\n";
		}
		key_space_lastState = true;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		key_space_lastState = false;
	}

	if (jump)
	{
		if (glfwGetTime() - jumpTime > 0.2)
		{
			jump = false;
			view_matrix = glm::translate(view_matrix, glm::vec3(0.0f, 1.0f, 0.0f));
		}
	}

	updateCameraVectors();
}

float last_yaw;
float last_pitch;
bool first = false;

void Camera::mouseInput(float xoffset, float yoffset)
{
	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	if (!first)
	{
		first = true;
	}
	else
	{
		front = glm::rotate(front, glm::radians(pitch - last_pitch), glm::vec3(1.0f, 0.0f, 0.0f));
		front = glm::rotate(front, glm::radians(yaw - last_yaw), glm::vec3(0.0f, -1.0f, 0.0f));
	}
	last_yaw = yaw;
	last_pitch = pitch;

	updateCameraVectors();
}
 

glm::mat4 Camera::get_viewMatrix() const
{
	return view_matrix;
}



void Camera::updateCameraVectors()
{
	//muismy ustawic tylko wetkor kierunku (zmieniamy front kamery)
	//glm::vec3 direction;
	//direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	//direction.y = sin(glm::radians(pitch));	
	//direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	
	//front = glm::normalize(direction);

	view_matrix = glm::lookAt(position, position + front, up_vector);
	
	
	if (jump) {
		view_matrix = glm::translate(view_matrix, glm::vec3(0.0f, -1.0f, 0.0f));
	}
}
