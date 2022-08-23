#include "../headers/Camera.h"

// constructor
Camera::Camera(glm::vec3 pos, glm::vec3 front_, glm::vec3 up_vec): position(pos), front(front_), up_vector(up_vec)
{
	// initialize camera matrix and yaw angle
	view_matrix = glm::lookAt(position, position + front, up_vector);
	yaw = -90.0f;
	speedFactor = 2.5f;
}

// destructor
Camera::~Camera()
{
}

// keyboard input callback function for camera movement
void Camera::Input(GLFWwindow * window, float deltaTime)
{
	float cameraSpeed = speedFactor * deltaTime;
	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position += front * cameraSpeed; // update camera position
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position -= front * cameraSpeed; // update camera position
	}
	//left and right vector are obtained from cross product of front vector and up vector
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position -= glm::normalize(glm::cross(up_vector, front)) * cameraSpeed; // update camera position
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position += glm::normalize(glm::cross(up_vector, front)) * cameraSpeed; // update camera position
	}
	
	updateCameraMatrix(); 
}

// mouse inout callback function
void Camera::mouseInput(float xoffset, float yoffset)
{
	// +xoffset -> right
	// +yoffset -> up
	yaw += xoffset; //mouse moved left <--> right
	pitch += yoffset; //mouse moved up <--> down

	// bounds
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

	front = glm::normalize(front);
	
	updateCameraMatrix();
}
 
// return camera's view matrix
glm::mat4 Camera::get_viewMatrix() const
{
	return view_matrix;
}

// update view matrix
void Camera::updateCameraMatrix()
{
	view_matrix = glm::lookAt(position, position + front, up_vector);
}

// return camera's position
glm::vec3 Camera::getCamPos() const
{
	return position;
}

// return camera's front vector
glm::vec3 Camera::getCamFront() const
{
	return front;
}
