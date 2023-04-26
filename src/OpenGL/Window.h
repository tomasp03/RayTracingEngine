#pragma once
#include <GLFW/glfw3.h>
#include <iostream>

class Window
{
private:
	GLFWwindow* m_window;

public:
	Window(int width, int height, const char* title);
	~Window();
	void SwapBuffers();
	void PollEvents();
	void Destroy();
	GLFWwindow* GetID();
};



