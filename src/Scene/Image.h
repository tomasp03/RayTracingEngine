#pragma once
#include <glm/glm.hpp>
#include <iostream>

struct Image
{
	uint32_t* data;
	glm::ivec2 size;
	std::string name;

	float AspectRatio() { return (float)size.x / (float)size.y; }
};