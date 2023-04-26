#pragma once
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace loader
{
	void init();
	void GLFW();
	void GladGL();
}