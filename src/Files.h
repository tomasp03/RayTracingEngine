#pragma once
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>

class Files
{
public:
	void Save(std::ofstream& stream, glm::vec3 vec);
	void Save(std::ofstream& stream, glm::vec4 vec);
};

