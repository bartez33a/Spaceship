#include <iostream>
#include <string>
#include <glad\glad.h> // glad before glfw!
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//my includes
#include "headers/Shader.h"
#include "headers/VAO.h"
#include "headers/VBO.h"
#include "headers/Rectangle.h"
#include "headers/Cube.h"
#include "headers/stb_image.h"
#include "headers/Camera.h"

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

// create FPS camera object 
// arguments: positon_of_camera, front_of_camera, up_vector
Camera cam1(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

int main()
{
	std::cout << "Welcome to Spacecraft game!\n";

	//initialize glfw
	initialize_glfw();
	
	//create window using glfw
	GLFWwindow* window = glfwCreateWindow(width, height, "Spacecraft", NULL, NULL);

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

	//create shader programs
	Shader shader("shaders/shader.vs", "shaders/shader.fs"); //shader without texture
	Shader shader_tex("shaders/shader_tex.vs", "shaders/shader_tex.fs"); //shader with texture

	//textures
	//anti load image upside-down
	stbi_set_flip_vertically_on_load(true);

	//no textures right now

	// view martix for camera
	glm::mat4 view = glm::mat4(1.0f);	
	//projection matrix
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	// create objects
	Cube c1(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f, 0.0f);

	//enable depth testing for 3d drawing
	glEnable(GL_DEPTH_TEST);

	//rendering loop
	while (!glfwWindowShouldClose(window))
	{
		//calculate time difference between 2 consecutive frames
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	
		//clear buffers
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//process inpout function e.g. exit application
		processInput(window);
		
		//use camera
		//use camera input and get viewMatrix
		cam1.Input(window, deltaTime);
		view = cam1.get_viewMatrix();

		//shader for cubes without texture
		shader.use();
		glm::mat4 mm = glm::mat4(1.0f);

		shader.setUniformMatrix(shader.getModelMatrixLocation(), mm);
		shader.setUniformMatrix(shader.getViewMatrixLocation(), view); //update camera
		shader.setUniformMatrix(shader.getProjectionMatrixLocation(), projection);
		c1.draw();
				
		glfwSwapBuffers(window);
		glfwPollEvents();
	} //end of render loop

	//terminate glfw
	glfwTerminate();

	return EXIT_SUCCESS;
}

// framebuffer callback function
// changes viewport when size of windows changes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


// porcess window inputs
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

}

//mouse input callback function
void mouseInput(GLFWwindow* window, double xpos, double ypos)
{
	static bool firstMouse = false; // to initialize mouse_lastXY positions
	static float mouse_lastX, mouse_lastY;
	
	// initialize last positions at start
	if (firstMouse) // initially set to true
	{
		mouse_lastX = xpos;
		mouse_lastY = ypos;
		firstMouse = false;
	}


	//mouse xpos -> positive to the right
	//mouse ypos -> positive to the bottom!
	float xoffset = xpos - mouse_lastX;
	float yoffset = mouse_lastY - ypos; // reversed: y ranges bottom to top
	mouse_lastX = xpos;
	mouse_lastY = ypos;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	//camera mouse input
	cam1.mouseInput(xoffset, yoffset);
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