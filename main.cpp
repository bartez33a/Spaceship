// TODO
// - visualize loading ammo time
// - add few more weapons
// - increase size of spaceship?
// - add menu e.g. for show best score, exit game

#include <iostream>
#include <string>
#include <glad\glad.h> // glad before glfw!
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <chrono>
#include <ctime>
#include <Windows.h>

//my includes
#include "headers/Camera.h"
#include "headers/Rocket.h"
#include "headers/Manager.h"

// my functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void initialize_glfw();
void initialize_glad();
void processInput(GLFWwindow *window);
void mouseInput(GLFWwindow* window, double xpos, double ypos);

// global variables
const int width = 800;
const int height = 600;

// deltatime - for movement of camera
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

//vecotr of rockets
Manager *manager_ptr;

int main()
{
	std::cout << "Welcome to Spacecraft game!\n";
	//initialize glfw
	initialize_glfw();
	
	//create window using glfw
	GLFWwindow* window = glfwCreateWindow(width, height, "Spacecraft", NULL, NULL);

	// if window wasnt created
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//make context with current window
	glfwMakeContextCurrent(window);
	
	//initialize glad
	initialize_glad();

	//viewport
	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	//mouse input
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //hide mouse cursor
	glfwSetCursorPosCallback(window, mouseInput); //set mouse callback function

	//game manager
	Manager manager(window);
	manager_ptr = &manager;

	//enable depth testing for 3d drawing
	glEnable(GL_DEPTH_TEST);

	int noOfFrames = 0;
	double lastTime = glfwGetTime();

	//rendering loop
	while (!glfwWindowShouldClose(window))
	{
		//calculate time difference between 2 subsequent frames
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	
		// Measure FPS
		noOfFrames++;
		// after each second
		if (currentFrame - lastTime >= 1.0) 
		{ 
			// printf and reset timer
			//std::cout << double(noOfFrames) << " FPS\n";
			noOfFrames = 0;
			lastTime = currentFrame;
		}

		//clear buffers
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (!manager.play(window, deltaTime))
		{
			//gameOver
			glfwSetWindowShouldClose(window, true);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	} //end of render loop

	//terminate glfw
	glfwTerminate();

	getchar(); getchar();

	return EXIT_SUCCESS;
}

// framebuffer callback function
// changes viewport when size of windows changes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//mouse input callback function
void mouseInput(GLFWwindow* window, double xpos, double ypos)
{
	static bool firstMouse = false; // to initialize mouse_lastXY positions
	static double mouse_lastX, mouse_lastY;
	
	// initialize last positions at start
	if (!firstMouse) // initially set to true
	{
		mouse_lastX = xpos;
		mouse_lastY = ypos;
		firstMouse = true;
	}

	//mouse xpos -> positive to the right
	//mouse ypos -> positive to the bottom!
	double xoffset = xpos - mouse_lastX;
	double yoffset = mouse_lastY - ypos; // reversed: y ranges bottom to top
	mouse_lastX = xpos;
	mouse_lastY = ypos;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	//camera mouse input
	manager_ptr->mouseInput(xoffset, yoffset);
}

void initialize_glfw()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void initialize_glad()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD\n";
		getchar();
		exit (-1);
	}
}