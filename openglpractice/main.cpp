#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

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
	{
		-0.5f,     -0.5f * float(sqrt(3)) / 3, 0.0f, // lower left
		 0.5f,     -0.5f * float(sqrt(3)) / 3, 0.0f, // lower right
		 0.0f,      1.0f * float(sqrt(3)) / 3, 0.0f, // upper
		-0.5f / 2,  0.5f * float(sqrt(3)) / 6, 0.0f, // inner left
		 0.5f / 2,  0.5f * float(sqrt(3)) / 6, 0.0f, // inner right
		 0.0f,     -0.5f * float(sqrt(3)) / 3, 0.0f  // inner down
	};

	GLuint indices[] =
	{
		0, 3, 5,  // lower left triangle
		3, 2, 4,  // lower right triangle
		5, 4, 1   // upper triangle
	};

	// create a window
	GLFWwindow* window = glfwCreateWindow(800, 800, "My Window", NULL, NULL);
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
	glViewport(0, 0, 800, 800);

	// creates shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to vertices
	EBO EBO1(indices, sizeof(indices));

	// Links VBO to VAO
	VAO1.LinkVBO(VBO1, 0);
	// Unbind to prevent accidentally modifying the objects
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		
		// specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// clear the back buffer and assign new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		shaderProgram.Activate();
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		// swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		
		// process all events while window is on
		glfwPollEvents();
	}

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}