#include "window.hpp"
#include "timer.hpp"
#include "input.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"

int main(void)
{
	Window::open();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplOpenGL3_Init();

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 0.0f;

	Timer frameTime;

	char buf[256];
	float f = 0;

	while (!Window::shouldClose())
	{
		glClear(GL_COLOR_BUFFER_BIT);

		double dt = frameTime.elapsed();

		io.DeltaTime = dt;
		io.DisplaySize.x = Window::size().x;
		io.DisplaySize.y = Window::size().y;
		io.MousePos.x = Input::mousePosition().x;
		io.MousePos.y = Input::mousePosition().y;
		io.MouseDown[0] = Input::isMouseButtonDown(GLFW_MOUSE_BUTTON_1);
		io.MouseDown[1] = Input::isMouseButtonDown(GLFW_MOUSE_BUTTON_2);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
		ImGui::Text("Hello, world %d", 123);
		if (ImGui::Button("Save"))
		{
			// do stuff
		}
		ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		ImGui::EndFrame();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		Window::update();
	}

	Window::close();
	return 0;
}