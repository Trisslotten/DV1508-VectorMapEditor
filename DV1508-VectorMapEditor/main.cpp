#include "window.hpp"
#include "timer.hpp"
#include "input.hpp"
#include "engine.hpp"

int main(void)
{
	Window::open();

	Engine engine;
	engine.init();

	while (!Window::shouldClose())
	{
		engine.update();
		engine.render();
		Window::update();
	}
	Window::close();
	return 0;
}