#pragma once

#include "renderer.hpp"

class Engine
{
public:
	void init();
	void update();
	void render();
private:
	void setImguiInput();

	void testWindow1();
	void testWindow2();

	Renderer renderer;
	VectorMap vmap;


	Timer frameTime;
};