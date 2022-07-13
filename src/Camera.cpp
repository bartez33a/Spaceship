#include "../headers/Camera.h"



Camera::Camera(glm::vec3 pos, glm::vec3 front_, glm::vec3 up_vec): position(pos), front(front_), up_vector(up_vec)
{
	// initialize camera matrix and yaw angle
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

	//jump
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (!key_space_lastState)
		{
			jump = true;
			jumpTime = glfwGetTime();
		}
		key_space_lastState = true;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		key_space_lastState = false;
	}

	updateCameraMatrix();
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

	updateCameraMatrix();
}
 

glm::mat4 Camera::get_viewMatrix() const
{
	return view_matrix;
}



void Camera::updateCameraMatrix()
{
	view_matrix = glm::lookAt(position, position + front, up_vector);
	
	if (jump) {
		//translate viewMatrix -> every call of this function viewMatrix is updated so we have to translate this matrix
		//every call of this function!
		view_matrix = glm::translate(view_matrix, glm::vec3(0.0f, -1.0f, 0.0f));

		if (glfwGetTime() - jumpTime > 0.2) // 200 ms
		{
			jump = false;
		}
	}
}
