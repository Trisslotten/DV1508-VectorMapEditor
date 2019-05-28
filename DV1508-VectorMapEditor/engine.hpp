#pragma once

#include "renderer.hpp"
#include "texture.hpp"
#include "minimap.hpp"
#include "tools.hpp"

class Engine
{
public:
	void init();
	void update();
	void render();
private:
	Texture cube;
	bool displayOrientationMenu = true;
	bool displayCameraSettings = false;
	bool displayMinimap = true;
	Minimap minimap;
	void setImguiInput();
	bool wireframe = false;
	void showMenuBar();
	void showToolsMenu();
	void showOrientationMenu();
	void showShadingMenu();
	void showMiniMap();
	void showCameraSettings();
	bool fpscam = false;

	bool canUseTool();

	Renderer renderer;
	VectorMap vmap;

	Timer frameTime;
	double dt;

	Tool* currentTool = nullptr;

	std::vector<Tool*> tools;


	bool brushCrossActive = false;
	glm::vec2 brushCrossNDC;

	struct
	{
		// Hack, don't use
		int active;
	} toolsMenu;

	struct
	{
		float radius = 1.f;
		float strength = 1.f;

		const float maxRadius= 10.f;
		const float maxStrength = 10.f;
	} brushSettings;


};