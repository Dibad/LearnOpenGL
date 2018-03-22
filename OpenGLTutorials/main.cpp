#include <iostream>

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#define NumOfElements(arr) sizeof(arr) / sizeof(arr[0])

// Callbacks
void framebuffer_size_callback(GLFWwindow * window, int width, int height);
void mouse_callback(GLFWwindow * window, double xpos, double ypos);
void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);

// Functions
void input(GLFWwindow * window);

// Constants
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Global variables
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f;


int main()
{
	// Init glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window
	GLFWwindow * window;

	try
	{
		window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);

		if (window == nullptr)
			throw "Failed to create GLFW window";

		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);

		// Tell glfw to capture mouse inside window
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// glad: Load opengl funciton pointer
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			throw "Failed to initialize GLAD";

		// Configure global opengl state: Check z-buffer
		glEnable(GL_DEPTH_TEST);
	}
	catch (const char * msg)
	{
		std::cout << "Error: " << msg << std::endl;
		glfwTerminate();
		return -1;
	}

	double init_x, init_y;
	glfwGetCursorPos(window, &init_x, &init_y);
	lastX = float(init_x);
	lastY = float(init_y);

	/////////////////////////////////////////////////////////////////////

	Shader ourShader("shaders/container.vs", "shaders/container.fs");

	Model ourModel("res/nanosuit/nanosuit.obj");

	//////////////////////////////////////////////////////////////////////

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		input(window);

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.use();

		glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.getViewMatrix();
		ourShader.set("projection", projection);
		ourShader.set("view", view);

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		ourShader.set("model", model);


		ourModel.draw(ourShader);

		////// SWAP /////
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}


// Query all input
void input(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(Movement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(Movement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(Movement::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(Movement::RIGHT, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.processKeyboard(Movement::UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.processKeyboard(Movement::DOWN, deltaTime);
}


// Called when window is resized
void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}


// Called when mouse moves
void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	float xoffset = float(xpos) - lastX;
	float yoffset = lastY - float(ypos); // Reversed since y-coordinates go from botton to top

	lastX = float(xpos);
	lastY = float(ypos);

	camera.processMouseMovement(xoffset, yoffset);
}


// Called when mouse wheel scrolls
void scroll_callback(GLFWwindow *, double xoffset, double yoffset)
{
	camera.processMouseScroll(yoffset);
}
