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

	bool canUseTool();

	void showGraphEditor();
	Renderer renderer;
	VectorMap vmap;

	Timer frameTime;
	double dt;

	Texture testIcon;

	Tool* currentTool = nullptr;

	std::vector<Tool*> tools;

	struct
	{
		// Hack, don't use
		int active;
	} toolsMenu;

	struct
	{
		float radius = 1.f;
		float strength = 1.f;
	} brushSettings;
};