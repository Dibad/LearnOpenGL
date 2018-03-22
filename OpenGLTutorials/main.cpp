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

#define NumOfElements(arr) sizeof(arr) / sizeof(arr[0])

// Callbacks
void framebuffer_size_callback(GLFWwindow * window, int width, int height);
void mouse_callback(GLFWwindow * window, double xpos, double ypos);
void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);

// Functions
GLuint loadTexture(const char * path);

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

bool on = true;


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


	// Build and compile Shader
	Shader lightingShader("shaders/container.vs", "shaders/container.fs");
	Shader lampShader("shaders/light.vs", "shaders/light.fs");

	// Set up vertex and buffer data and configure VAO
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};
	// positions of all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	// positions of the point lights
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	glm::vec3 pointLightColors[] = {
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0.4f, 0.1f, 0.1f)
	};


	GLuint VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(cubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Texture coords attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	///////////////////////////////////////////////////////////////////////////

	// load Texture
	GLuint diffuseMap = loadTexture("container2.png");
	GLuint specularMap = loadTexture("container2_specular.png");
	// Set material properties
	lightingShader.use();
	lightingShader.set("material.diffuse", 0);
	lightingShader.set("material.specular", 1);
	lightingShader.set("material.shininess", 64.0f);

	///////////////////////////////////////////////////////////////////////////

	// Create light VAO and set uniform
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	////////////////////////////////////////////////////////////////////////////////


	while (!glfwWindowShouldClose(window))
	{
		// Per-frame time logic
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		input(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Shader position
		lightingShader.use();
		lightingShader.set("viewPos", camera.getPosition());

		// directional light
		lightingShader.set("dirLight.direction", -0.2f, -1.0f, -0.3f);
		lightingShader.set("dirLight.ambient", 0.0f, 0.0f, 0.0f);
		lightingShader.set("dirLight.diffuse", 0.05f, 0.05f, 0.05f);
		lightingShader.set("dirLight.specular", 0.05f, 0.05f, 0.05f);

		for (int i = 0; i < NumOfElements(pointLightPositions); ++i)
		{
			std::string buffer = "pointLights[" + std::to_string(i) + "].";
			
			lightingShader.set(buffer + "position", pointLightPositions[i]);
			lightingShader.set(buffer + "ambient", pointLightColors[i] * 0.1f);
			lightingShader.set(buffer + "diffuse", pointLightColors[i]);
			lightingShader.set(buffer + "specular", pointLightColors[i]);
			
			lightingShader.set(buffer + "constant", 1.0f);
			lightingShader.set(buffer + "linear", 0.14f);
			lightingShader.set(buffer + "quadratic", 0.07f);
		}

		// spotLight
		lightingShader.set("spotLight.position", camera.getPosition());
		lightingShader.set("spotLight.direction", camera.getFront());
		lightingShader.set("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		lightingShader.set("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		lightingShader.set("spotLight.specular", 1.0f, 1.0f, 1.0f);
		lightingShader.set("spotLight.constant", 1.0f);
		lightingShader.set("spotLight.linear", 0.09f);
		lightingShader.set("spotLight.quadratic", 0.032f);
		lightingShader.set("spotLight.cutOff", glm::cos(glm::radians(10.0f)));
		lightingShader.set("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		lightingShader.set("on", on);

		// View/Projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.getViewMatrix();
		lightingShader.set("projection", projection);
		lightingShader.set("view", view);

		// World transformations
		glm::mat4 model;

		// Texture setting
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		// Render cube
		glBindVertexArray(cubeVAO);

		for (unsigned int i = 0; i < NumOfElements(cubePositions); ++i)
		{
			model = glm::mat4();
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightingShader.set("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// Draw lamp object
		lampShader.use();
		lampShader.set("projection", projection);
		lampShader.set("view", view);

		glBindVertexArray(lightVAO);
		for (unsigned int i = 0; i < NumOfElements(pointLightPositions); ++i)
		{
			lampShader.set("lightColor", pointLightColors[i]);

			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			lampShader.set("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		////// SWAP /////
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

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

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		on = true;

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
		on = false;
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



GLuint loadTexture(const char * path)
{
	GLuint texture;
	glGenTextures(1, &texture);

	// Load image
	int widht, height, nrComponents;
	unsigned char * data = stbi_load(path, &widht, &height, &nrComponents, 0);

	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;;

		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(GL_TEXTURE_2D, 0, format, widht, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Set texture wrapping options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
		std::cout << "Failed to load texture " << path << std::endl;

	stbi_image_free(data);

	return texture;
}
