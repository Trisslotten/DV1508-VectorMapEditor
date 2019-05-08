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
#include "renderer.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"

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
	

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplOpenGL3_Init();
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 0.0f;

	char buf[256] = {};
	float f = 0;


	Renderer renderer;
	renderer.init();

	VectorMap vmap;
	vmap.init(); // vmap.init(128);
	renderer.setVectorMap(&vmap);




	glClearColor(0.5, 0.5, 0.5, 1.0);

	Timer frameTime;
	while (!Window::shouldClose())
	{

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double dt = frameTime.elapsed();

		io.DeltaTime = (float)dt;
		io.DisplaySize.x = Window::size().x;
		io.DisplaySize.y = Window::size().y;
		io.MousePos.x = Input::mousePosition().x;
		io.MousePos.y = Input::mousePosition().y;
		io.MouseDown[0] = Input::isMouseButtonDown(GLFW_MOUSE_BUTTON_1);
		io.MouseDown[1] = Input::isMouseButtonDown(GLFW_MOUSE_BUTTON_2);


		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		if (ImGui::Begin("ImGui Demo"))
		{
			ImGui::Text("Hello, world %d", 123);

			for (int i = 0; i < 9; i++)
			{
				if(i % 3 != 0)
					ImGui::SameLine();
				std::string buttonName = "TestButton" + std::to_string(i+1);
				if (ImGui::Button(buttonName.c_str()))
				{
					// do stuff
				}
			}
			ImGui::InputText("hej", buf, IM_ARRAYSIZE(buf));
			ImGui::SliderFloat("san", &f, 0.0f, 1.0f);
			ImGui::BulletText("asdlfkjasfoliasje.");
		}
		ImGui::End();

		if (ImGui::Begin("bogdan window"))
		{
			ImGui::Text("asdasdas %d", 1337);
			ImGui::BulletText("Double-click on title bar to collapse window.");
		}
		ImGui::End();



		ImGui::EndFrame();


		renderer.render();


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		Window::update();
	}

	Window::close();
	return 0;
}