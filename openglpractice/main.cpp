#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

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
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f
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

	// create vertex shader object and get reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// attach vertex shader source to vertex shader object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// compile shader into machine code
	glCompileShader(vertexShader);

	// check if vertexShader has successfully compiled
	int successVertex;
	char infoLogVertex[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successVertex);

	if (!successVertex)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLogVertex);
		std::cout << "ERROR" << infoLogVertex << std::endl;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// check if fragmentShader has successfully compiled
	int successFragment;
	char infoLogFragment[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &successFragment);

	if (!successFragment)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLogFragment);
		std::cout << "ERROR" << infoLogFragment << std::endl;
	}

	// create shader program object and get reference
	GLuint shaderProgram = glCreateProgram();
	// attach vertex and fragment shaders to shader program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// wrap up/link all shaders together into the shader program
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// create reference containers for 
	// vertex array and vertex buffer objects
	GLuint VAO, VBO;

	// ALWAYS generate VAOs before VBOs !!!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// make VAO the current vertex array object by binding it
	glBindVertexArray(VAO);

	// bind the VBO specifying its a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// introduce the vertices into the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// configure the vertex attribute so opengl knows how to read the vbo
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// bind both VBO and VAO to 0 so we don't modify it by mistake
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		
		// specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// clear the back buffer and assign new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		
		// process all events while window is on
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}