#pragma once

#include "renderer.hpp"
#include "texture.hpp"
#include "minimap.hpp"

class Engine
{
public:
	void init();
	void update();
	void render();
private:
	Minimap minimap;
	void setImguiInput();
	bool wireframe = false;
	void showMenuBar();
	void showToolsMenu();
	void showOrientationMenu();
	void showShadingMenu();
	void showMiniMap();
	void showGraphEditor();
	Renderer renderer;
	VectorMap vmap;

	Timer frameTime;

	Texture testIcon;

	struct
	{
		// Hack, don't use
		int active;
	} toolsMenu;

	struct
	{
		float size = 1.f;
		float strength = 1.f;
	} brushSettings;
};