#include "engine.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"

#include "input.hpp"

void Engine::init()
{
	ImGui::CreateContext();
	ImGui_ImplOpenGL3_Init();
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 0.0f;

	renderer.init();

	vmap.init(); // vmap.init(128);
	renderer.setVectorMap(&vmap);

	glClearColor(0.5, 0.5, 0.5, 1.0);
}

void Engine::update()
{
	setImguiInput();

	ImGui_ImplOpenGL3_NewFrame();

	ImGui::NewFrame();

	testWindow1();
	testWindow2();

	ImGui::EndFrame();
}

void Engine::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderer.render();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Engine::setImguiInput()
{
	double dt = frameTime.restart();

	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = (float)dt;
	io.DisplaySize.x = Window::size().x;
	io.DisplaySize.y = Window::size().y;
	io.MousePos.x = Input::mousePosition().x;
	io.MousePos.y = Input::mousePosition().y;
	io.MouseDown[0] = Input::isMouseButtonDown(GLFW_MOUSE_BUTTON_1);
	io.MouseDown[1] = Input::isMouseButtonDown(GLFW_MOUSE_BUTTON_2);
}

void Engine::testWindow1()
{
	if (ImGui::Begin("ImGui Demo"))
	{
		ImGui::Text("Hello, world %d", 123);

		for (int i = 0; i < 9; i++)
		{
			if (i % 3 != 0)
				ImGui::SameLine();
			std::string buttonName = "TestButton" + std::to_string(i + 1);
			if (ImGui::Button(buttonName.c_str()))
			{
				// do stuff
			}
		}
		//ImGui::InputText("hej", buf, IM_ARRAYSIZE(buf));
		//ImGui::SliderFloat("san", &f, 0.0f, 1.0f);
		ImGui::BulletText("asdlfkjasfoliasje.");
	}
	ImGui::End();
}

void Engine::testWindow2()
{
	if (ImGui::Begin("bogdan window"))
	{
		ImGui::Text("asdasdas %d", 1337);
		ImGui::BulletText("Double-click on title bar to collapse window.");
	}
	ImGui::End();
}
