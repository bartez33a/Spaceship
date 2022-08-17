// TODO
// - create shader for rockets
// - bound rockets number, bound time between two consecutive shoots
// - fuel for spaceship
// - 


#include <iostream>
#include <string>
#include <glad\glad.h> // glad before glfw!
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

//my includes
#include "headers/Shader.h"
#include "headers/VAO.h"
#include "headers/VBO.h"
#include "headers/Cube.h"
#include "headers/stb_image.h"
#include "headers/Camera.h"
#include "headers/meteor.h"
#include "headers/Rocket.h"
#include "headers/Manager.h"
#include "headers/Sphere.h"
#include "headers/Texture.h"
#include "headers/Point.h"

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
Shader *shader_ptr;
Manager manager;
std::list<Sphere> sphere_list;

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

	//create sahders
	//create shader programs
	Shader shader("shaders/shader.vs", "shaders/shader.fs"); //shader without texture
	shader_ptr = &shader;

	//shader for meteors
	Shader meteor_shader("shaders/shader_meteor_tex.vs", "shaders/shader_meteor_tex.fs");

	//shader for background meteors
	Shader background_meteors_shader("shaders/shader_background_meteor.vs","shaders/shader_background_meteor.fs");

	//textures
	Texture t1("textures/magma.png", GL_TEXTURE0);
	Texture t2("textures/meteor.png", GL_TEXTURE1);
	Texture t3("textures/meteor2.png", GL_TEXTURE2);

	manager.setMeteorsTexNo(3);

	//use textures
	//use program to set uniforms for textures
	meteor_shader.use();
	//0 ->  glActiveTexture(GL_TEXTURE0); // activate texture unit 0
	glUniform1i(glGetUniformLocation(meteor_shader.get_ID(), "texture0"), 0); // manually
	glUniform1i(glGetUniformLocation(meteor_shader.get_ID(), "texture1"), 1); // manually
	glUniform1i(glGetUniformLocation(meteor_shader.get_ID(), "texture2"), 2); // manually

	// view martix for camera
	glm::mat4 view = glm::mat4(1.0f);	
	//projection matrix
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 200.0f);

	//enable depth testing for 3d drawing
	glEnable(GL_DEPTH_TEST);

	double timer_meteor;
	timer_meteor = glfwGetTime();
	srand((unsigned int)time(NULL));


	//background 
	manager.createBackground(&background_meteors_shader);

	//bind textures before rendering loop -if you dont bind texture, shape will be black.
	t1.bindTexture();
	t2.bindTexture();
	t3.bindTexture();

	//rendering loop
	while (!glfwWindowShouldClose(window))
	{
		//calculate time difference between 2 subsequent frames
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	
		//clear buffers
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//process inpout function e.g. exit application
		processInput(window);

		//use camera input and get viewMatrix
		//cam1.Input(window, deltaTime);
		//view = cam1.get_viewMatrix();

		view = manager.getViewMatrix();

		//shader for cubes without texture
		shader.use();
		glm::mat4 mm = glm::mat4(1.0f);
		shader.setUniformMatrix(shader.getModelMatrixLocation(), mm);
		shader.setUniformMatrix(shader.getViewMatrixLocation(), view); //update camera
		shader.setUniformMatrix(shader.getProjectionMatrixLocation(), projection);
		
		//update shader's matices
		// shader for meteors
		meteor_shader.use();
		meteor_shader.updateMatrices(mm, view, projection);
	
		// shader for background meteors
		background_meteors_shader.use();
		background_meteors_shader.updateMatrices(mm, view, projection);

		// create meteors
		if ((glfwGetTime() - timer_meteor) > 1.0)
		{
			timer_meteor = glfwGetTime();
			manager.createMeteors(&meteor_shader);
		}


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

	getchar();

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
	static bool space_pushed = false;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
		
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (!space_pushed) //positive edge
		{
			manager.createRocket(shader_ptr);
		}
		space_pushed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		space_pushed = false;
	}
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
	manager.mouseInput(xoffset, yoffset);
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