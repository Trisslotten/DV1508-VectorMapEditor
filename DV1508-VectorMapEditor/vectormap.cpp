#include "vectormap.hpp"

// iq's integer hash https://www.shadertoy.com/view/llGSzw
float uhash12(glm::uvec2 x)
{
	glm::uvec2 q = 1103515245U * ((x >> 1U) ^ (glm::uvec2(x.y, x.x)));
	glm::uint  n = 1103515245U * ((q.x) ^ (q.y >> 3U));
	return float(n) * (1.0 / float(0xffffffffU));
}
float hash12(glm::vec2 x) { return uhash12(glm::uvec2(50.f*x)); }
float noise(glm::vec2 p)
{
	glm::vec2 ip = glm::floor(p);
	glm::vec2 u = glm::fract(p);
	u = u * u*(3.0f - 2.0f*u);

	float res = glm::mix(
		glm::mix(hash12(ip), hash12(ip + glm::vec2(1.0f, 0.0f)), u.x),
		glm::mix(hash12(ip + glm::vec2(0.0f, 1.0f)), hash12(ip + glm::vec2(1.0f, 1.0f)), u.x), u.y);
	return 2.f * res*res - 1.f;
}

glm::vec3 noise3(glm::vec2 p)
{
	glm::vec3 result;
	result.x = noise(p);
	result.y = noise(p + 100.f);
	result.z = noise(p - 100.f);
	return result;
}



void VectorMap::init(int size)
{
	this->size = size;
	vectorMap.resize(size*size);


	for (int i = 0; i < vectorMap.size(); i++)
	{
		int x = i % size;
		int y = i / size;

		glm::vec2 uv = glm::vec2(x, y) / float(size-1);

		vectorMap[i] = glm::vec3(0);
		float a = 1;
		float f = 4;
		for (int j = 0; j < 10; j++)
		{
			vectorMap[i] += a*noise3(f*uv);
			a *= 0.3;
			f /= 0.5;
		}
		
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, size, size, 0, GL_RGB, GL_FLOAT, vectorMap.data());
	glBindTexture(GL_TEXTURE_2D, 0);
}

void VectorMap::bind(int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texture);
}
