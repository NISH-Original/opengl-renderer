#include "Model.h"

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

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	// enable depth buffer
	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	Model model("C:/Users/nisha/Documents/Nishant/ProgrammingProjects/OpenGL/openglpractice/models/bunny/scene.gltf");

	double prevTime = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		
		// specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// clear the back buffer and depth buffer
		// and assign new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double currTime = glfwGetTime();
		double elapTime = currTime - prevTime;
		prevTime = currTime;
		camera.Inputs(window, elapTime);

		// updates and inputs camera matrix into vertex shader
		camera.UpdateMatrix(45.0f, 0.1f, 100.0f);

		model.Draw(shaderProgram, camera);

		// swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		
		// process all events while window is on
		glfwPollEvents();
	}

	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}