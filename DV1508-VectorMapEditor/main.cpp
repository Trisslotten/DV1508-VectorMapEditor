/*#ifdef _WIN64
#include <windows.h>
#include <filesystem>
#include "Shlwapi.h"
#include <iostream>
#pragma comment(lib, "Shlwapi.lib")
#endif // _WIN64*/
#include "window.hpp"
#include "timer.hpp"
#include "input.hpp"
#include "engine.hpp"

/*void setExecutablePath()
{
#ifdef _WIN64
	TCHAR path[MAX_PATH];
	if (GetModuleFileName(NULL, path, MAX_PATH))
	{
		PathRemoveFileSpec(path); // remove executable name
	}
	std::basic_string<TCHAR> bPath(path);
	std::experimental::filesystem::path p(bPath);
	std::experimental::filesystem::current_path(p);

#else

#endif // _WIN64

}*/
int main(void)
{
	//setExecutablePath();
	Window::open();

	Input::initialize();

	Engine engine;
	engine.init();

	while (!Window::shouldClose())
	{
		engine.update();
		engine.render();
		Window::update();
		Input::reset();
	}
	Window::close();
	return 0;
}