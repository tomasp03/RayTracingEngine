#include "ToString.h"

std::string converter::ToString(glm::vec3 vec)
{
	std::string x = std::to_string(vec.x);
	std::string y = std::to_string(vec.y);
	std::string z = std::to_string(vec.z);
	
	std::string result = "vec3(" + x + ", " + y + ", " + z + ")";

	return result;
}

std::string converter::ToString(glm::vec4 vec)
{
	std::string x = std::to_string(vec.x);
	std::string y = std::to_string(vec.y);
	std::string z = std::to_string(vec.z);
	std::string w = std::to_string(vec.w);

	std::string result = "vec4(" + x + ", " + y + ", " + z + ", " + w + ")";

	return result;
}
