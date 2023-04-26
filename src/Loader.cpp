#include "Loader.h"

void loader::init()
{
	GLFW();
	GladGL();
}

void loader::GLFW()
{
	if (!glfwInit())
	{
		std::cout << "Error: Failed to initialize GLFW!" << std::endl;
		glfwTerminate();
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
}

void loader::GladGL()
{
	if (!gladLoadGL())
	{
		std::cout << "Error: Failed to load glad!" << std::endl;
		glfwTerminate();
		return;
	}
}