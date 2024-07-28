#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

const unsigned int width = 800;
const unsigned int height = 800;

// process all input on window
void processInput(GLFWwindow* window)
{
	// close window if esc key pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	// initialize GLFW
	glfwInit();

	// tell glfw what opengl version we're using (v 3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// tell glfw we're using the core profile
	// so we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// declaring the vertices of the triangle
	GLfloat vertices[] =
	{ //     COORDINATES     /        COLORS      /   TexCoord  //
		-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
		 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 1.0f,
		 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	1.0f, 1.0f,
		 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	0.5f, 0.5f
	};

	GLuint indices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 1, 4,
		1, 2, 4,
		2, 3, 4,
		3, 0, 4
	};

	// create a window
	GLFWwindow* window = glfwCreateWindow(width, height, "My Window", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to load window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// introduce the window to the current context
	glfwMakeContextCurrent(window);

	// load GLAD so it configures opengl
	gladLoadGL();

	// specify the viewport of opengl in the window
	// here the viewport goes from (0, 0) to (800, 800)
	glViewport(0, 0, width, height);

	// creates shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to vertices
	EBO EBO1(indices, sizeof(indices));

	// Links VBO to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// Unbind to prevent accidentally modifying the objects
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// gets ID of uniform called 'scale'
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	// Texture 

	Texture obama("obama_pyramid.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	obama.texUnit(shaderProgram, "tex0", 0);

	// variables that help object rotate
	float rotation = 0.0f;
	double prevTime = glfwGetTime();

	// enable depth buffer
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		
		// specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// clear the back buffer and depth buffer
		// and assign new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderProgram.Activate();

		// simple timer
		double currTime = glfwGetTime();
		if (currTime - prevTime >= 1 / (double) 60)
		{
			rotation += 0.5f;
			prevTime = currTime;
		}

		// initialise matrices
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view  = glm::mat4(1.0f);
		glm::mat4 proj  = glm::mat4(1.0f);

		// assign different transformations to each matrix
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
		proj = glm::perspective(glm::radians(45.0f), (float) width / height, 0.1f, 100.0f);

		// outputs matrices into vertex shader
		int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		// assign value to the uniform
		// MUST ALWAYS DO AFTER ACTIVATING SHADER PROGRAM
		glUniform1f(uniID, 0.5f);
		obama.Bind();
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
		// swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		
		// process all events while window is on
		glfwPollEvents();
	}

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	obama.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}