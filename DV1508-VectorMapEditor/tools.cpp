#include "tools.hpp"
#include <glm/glm.hpp>
#include "vectormap.hpp"
#include "imgui/imgui.h"
#include "Bezier.hpp"

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

///////////////////////////////////////////////////
///////////////////////////////////////////////////
///////////////////////////////////////////////////

void ToolCurve::use(GLuint mouseUVSSBO, float radius, float strength)
{
	if (vectorMap)
	{
		curveShader.use();

		auto vMapSize = vectorMap->getSize();

		curveShader.uniform("radius", radius);
		curveShader.uniform("strength", strength);
		curveShader.uniform("imgSize", vMapSize);
		curveShader.uniformv("data", 4, currBezier);

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

bool ToolCurve::hasGUI()
{
	return false;
}

void ToolCurve::showGUI()
{
}

bool ToolCurve::hasSpecialGUI()
{
	return true;
}

void ToolCurve::showSpecialGUI()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
	if (ImGui::Begin("Graph editor", 0, window_flags)) {
		Bezier::bezier(currBezier);

		if (ImGui::BeginCombo("Saved curves", currBezierName)) {
			for (int i = 0; i < savedBeziers.size(); i++) {
				bool is_selected = (currBezier == savedBeziers.at(i).data);
				if (ImGui::Selectable(savedBeziers.at(i).name.c_str(), is_selected)) {
					currBezier = savedBeziers.at(i).data;
					currBezierName = savedBeziers.at(i).name.c_str();
						if (is_selected) {
							ImGui::SetItemDefaultFocus();
						}
				}
			}
			ImGui::EndCombo();
		}

	} ImGui::End();
}

std::string ToolCurve::name()
{
	return "Curve";
}

void ToolCurve::vInit()
{
	currBezier = new float[4];
	currBezierName = "No curve selected";
	for (int i = 0; i < 4; i++)
	{
		if (i < 2)
		{
			currBezier[i] = 0.f;
		}
		else
		{
			currBezier[i] = 1.f;
		}
	}
	for (int i = 0; i < 5; i++) {
		float* b = new float[4];
		for (int j = 0; j < 4; j++)
		{
			if (j < 2)
			{
				b[j] = (float) (i / 5.f);
			}
			else
			{
				b[j] = 1.f;
			}
		}
		std::stringstream ss;
		ss << "Curve_" << i;
		savedBeziers.push_back({ b, ss.str() });
	}
	curveShader.add("curve.comp");
	curveShader.compile();
}

std::string ToolCurve::iconFile()
{
	return "icon_tool_curve.png";
}
