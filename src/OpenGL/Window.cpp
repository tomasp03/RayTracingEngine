#include "Window.h"

static void KeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

Window::Window(int width, int height, const char* title)
{

	m_window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!m_window)
	{
		std::cout << "Error: Failed to create window!" << std::endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_window);
	glfwSetKeyCallback(m_window, KeyPressed);
}

Window::~Window()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(m_window);
}

void Window::PollEvents()
{
	glfwPollEvents();
}

void Window::Destroy()
{
	glfwDestroyWindow(m_window);
}

GLFWwindow* Window::GetID()
{
	return m_window;
}

