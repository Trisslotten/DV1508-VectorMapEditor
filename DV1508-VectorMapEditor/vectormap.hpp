#pragma once

#include <vector>
#include <glm/glm.hpp>

class VectorMap
{
public:
	void init(int size = 128);
private:
	int size = -1;
	std::vector<glm::vec3> vectorMap;
};