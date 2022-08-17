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
			//jump = true;
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


void Camera::mouseInput(float xoffset, float yoffset)
{
	// +xoffset -> right
	// +yoffset -> up
	yaw += xoffset; //mouse moved left <--> right
	pitch += yoffset; //mouse moved up <--> down

	//std::cout << "xoffset: " << xoffset << " yoffset = " << yoffset << '\n';

	// bounds
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

	//std::cout << "Front: " << front.x << ",  " << front.y << ", " << front.z << "\n";
	//std::cout << "yaw = " << yaw << " pitch = " << pitch << "\n";

	front = glm::normalize(front);
	
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

glm::vec3 Camera::getCamPos() const
{
	return position;
}

glm::vec3 Camera::getCamFront() const
{
	return front;
}
