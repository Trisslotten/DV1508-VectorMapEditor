#pragma once
#include "lodepng.h"
#include <vector>
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <unordered_map>

class Texture
{
private:
	GLuint id = 0;
	unsigned int width = 0, height = 0;

	std::unordered_map<GLenum, GLint> hints;
public:
	Texture();
	void hint(GLenum pname, GLint param);
	bool loadTexture(const std::string& file);
	void bind(unsigned int slot) const;
	GLuint getID();
};
