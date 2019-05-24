#include "tools.hpp"
#include <glm/glm.hpp>
#include "vectormap.hpp"


void ToolAddHeight::vInit()
{
	addHeightShader.add("addheight.comp");
	addHeightShader.compile();
}

void ToolAddHeight::use(glm::vec2 mouseUV, float radius, float strength)
{
	if (vectorMap)
	{
		addHeightShader.use();

		auto vMapSize = vectorMap->getSize();

		addHeightShader.uniform("brushUV", mouseUV);
		addHeightShader.uniform("radius", radius);
		addHeightShader.uniform("strength", strength);
		addHeightShader.uniform("imgSize", vMapSize);

		vectorMap->bindAsImage(1);

		float size = glm::ceil(2.f * radius * vMapSize.x);
		float groupSize = 16.f;
		int numGroups = glm::ceil(size / groupSize);
		glDispatchCompute(numGroups, numGroups, 1);
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
