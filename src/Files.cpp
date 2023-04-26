#include "Files.h"

void Files::Save(std::ofstream& stream, glm::vec3 vec)
{
	stream << "vec3(";
	stream << vec.x << ", ";
	stream << vec.y << ", ";
	stream << vec.z << ")";
}

void Files::Save(std::ofstream& stream, glm::vec4 vec)
{
	stream << "vec4(";
	stream << vec.x << ", ";
	stream << vec.y << ", ";
	stream << vec.z << ", ";
	stream << vec.w << ")";
}
