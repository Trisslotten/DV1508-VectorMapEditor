#include "tools.hpp"
#include <glm/glm.hpp>
#include "vectormap.hpp"


void ToolAddHeight::vInit()
{
	addHeightShader.add("addheight.comp");
	addHeightShader.compile();
}

void ToolAddHeight::use(GLuint mouseUVSSBO, float radius, float strength)
{
	if (vectorMap)
	{
		addHeightShader.use();

		auto vMapSize = vectorMap->getSize();

		addHeightShader.uniform("radius", radius);
		addHeightShader.uniform("strength", strength);
		addHeightShader.uniform("imgSize", vMapSize);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, mouseUVSSBO);

		vectorMap->bindAsImage(2);

		float size = glm::ceil(2.f * radius * vMapSize.x);
		float groupSize = 16.f;
		int numGroups = glm::ceil(size / groupSize);
		glDispatchCompute(numGroups, numGroups, 1);

		glBindImageTexture(2, 0, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

		glMemoryBarrier(GL_ALL_BARRIER_BITS);
	}
}

bool ToolAddHeight::hasGUI()
{
	return false;
}

void ToolAddHeight::showGUI()
{

}

bool ToolAddHeight::hasSpecialGUI()
{
	return false;
}

void ToolAddHeight::showSpecialGUI()
{

}

///////////////////////////////////////////////////
///////////////////////////////////////////////////
///////////////////////////////////////////////////

void ToolSmoothen::use(GLuint mouseUVSSBO, float radius, float strength)
{

}

bool ToolSmoothen::hasGUI()
{
	return false;
}

void ToolSmoothen::showGUI()
{
}

bool ToolSmoothen::hasSpecialGUI()
{
	return false;
}

void ToolSmoothen::showSpecialGUI()
{
}

void ToolSmoothen::vInit()
{

}

///////////////////////////////////////////////////
///////////////////////////////////////////////////
///////////////////////////////////////////////////

void ToolExpand::use(GLuint mouseUVSSBO, float radius, float strength)
{
	if (vectorMap)
	{
		expandShader.use();

		auto vMapSize = vectorMap->getSize();

		expandShader.uniform("radius", radius);
		expandShader.uniform("strength", strength);
		expandShader.uniform("imgSize", vMapSize);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, mouseUVSSBO);

		vectorMap->bindAsImage(2);

		float size = glm::ceil(2.f * radius * vMapSize.x);
		float groupSize = 16.f;
		int numGroups = glm::ceil(size / groupSize);
		glDispatchCompute(numGroups, numGroups, 1);

		glBindImageTexture(2, 0, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

		glMemoryBarrier(GL_ALL_BARRIER_BITS);
	}
}

bool ToolExpand::hasGUI()
{
	return false;
}

void ToolExpand::showGUI()
{
}

bool ToolExpand::hasSpecialGUI()
{
	return false;
}

void ToolExpand::showSpecialGUI()
{
}

void ToolExpand::vInit()
{
	expandShader.add("expand.comp");
	expandShader.compile();
}

std::string ToolExpand::iconFile()
{
	return "icon_tool_expand.png";
}
