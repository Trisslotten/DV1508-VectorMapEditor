#include "engine.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"

#include "input.hpp"

#include "Bezier.hpp"

void Engine::init()
{
	ImGui::CreateContext();
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsLight();
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 0.0f;

	renderer.init();

	vmap.init(); // vmap.init(128);
	renderer.setVectorMap(&vmap);

	// add tools
	tools.push_back(new ToolAddHeight());
	for (auto tool : tools)
	{
		tool->init();
	}

	testIcon.loadTexture("assets/icon_tool_up.png");

	glm::vec3 clearColor{ 0.7f };
	glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0);
}

void Engine::update()
{
	dt = frameTime.restart();

	setImguiInput();

	ImGui_ImplOpenGL3_NewFrame();

	ImGui::NewFrame();

	showMenuBar();
	showToolsMenu();
	showOrientationMenu();
	showShadingMenu();
	showMiniMap();
	showGraphEditor();
	//ImGui::ShowDemoWindow();

	ImGui::EndFrame();

	
	if (canUseTool())
	{
		glm::vec2 mouseUV = renderer.mouseTerrainIntersection();
		
		float radius = brushSettings.radius * 0.02f;
		float strength = brushSettings.strength * 0.5f * dt * radius;
		
		renderer.showBrush(mouseUV, radius);

		if (Input::isMouseButtonDown(GLFW_MOUSE_BUTTON_1) && currentTool)
		{
			currentTool->setVectorMap(&vmap);
			currentTool->use(mouseUV, radius, strength);
		}
	}
}

void Engine::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	minimap.bindFBO();
	renderer.renderMiniMap();
	minimap.unbindFBO();
	renderer.render();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void Engine::setImguiInput()
{
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = (float)dt;
	io.DisplaySize.x = Window::size().x;
	io.DisplaySize.y = Window::size().y;
	io.MousePos.x = Input::mousePosition().x;
	io.MousePos.y = Input::mousePosition().y;
	io.MouseDown[0] = Input::isMouseButtonDown(GLFW_MOUSE_BUTTON_1);
	io.MouseDown[1] = Input::isMouseButtonDown(GLFW_MOUSE_BUTTON_2);
	io.MouseWheel = Window::mouseScroll().y;
}

void Engine::showMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New", "Ctrl+N")) {}
			if (ImGui::MenuItem("Open", "Ctrl+O")) {}
			if (ImGui::MenuItem("Save", "Ctrl+S")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Quit", "Ctrl+Q")) 
			{
				exit(0);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Tools", "")) {}
			if (ImGui::MenuItem("Orientation", "")) {}
			if (ImGui::MenuItem("Shading", "")) {}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void Engine::showGraphEditor() {
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
	if (ImGui::Begin("Graph editor", 0, window_flags)) {
		float arr[] = { 0.f, 0.f, 0.1f, 1.f };
		Bezier::bezier("Pingas", arr);
	} ImGui::End();
}

void Engine::showMiniMap()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

	ImTextureID tex = reinterpret_cast<ImTextureID>(minimap.getTextureID());
	if (ImGui::Begin("Minimap", 0, window_flags))
	{
		ImGui::Image(tex, ImVec2(300, 300));
	}
	ImGui::End();
}
bool Engine::canUseTool()
{
	return !ImGui::GetIO().WantCaptureMouse && Window::cursorIsEnabled();
}
void Engine::showToolsMenu()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

	if (ImGui::Begin("Tools", 0, window_flags))
	{
		for (int i = 0; i < tools.size(); i++)
		{
			if (i % 4 != 0)
				ImGui::SameLine();

			ImTextureID tex = reinterpret_cast<ImTextureID>(tools[i]->getIconID());
			ImGui::PushID(tools[i]->getIconID());
			if (ImGui::ImageButton(tex, ImVec2(32, 32)))
			{
				currentTool = tools[i];
			}
			ImGui::PopID();
		}

		ImGui::Separator();
		//ImGui::Text("Brush Settings");
		ImGui::PushItemWidth(100);

		ImGui::SliderFloat("Size", &brushSettings.radius, 1, 10, "%.f");
		ImGui::SliderFloat("Strength", &brushSettings.strength, 1, 10, "%.f");
	}
	ImGui::End();
}

void Engine::showOrientationMenu()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
	window_flags |= ImGuiWindowFlags_NoMove;
	if (ImGui::Begin("Orientation", 0, window_flags))
	{
		ImGui::Text("   @ . . . . @   ");
		ImGui::Text("   .\\        .\\   ");
		ImGui::Text("   . \\       . \\   ");
		ImGui::Text("   .  @ . . ... @   ");
		ImGui::Text("   .  .      .  .   ");
		ImGui::Text("   @ ... . . @  .   ");
		ImGui::Text("    \\ .       \\ .   ");
		ImGui::Text("     \\.        \\.   ");
		ImGui::Text("      @ . . . . @   ");
	}
	ImGui::End();

}

void Engine::showShadingMenu()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
	if (ImGui::Begin("Shading", 0, window_flags))
	{
		ImGui::Button("Shaded");
		ImGui::Button("Textured");
		ImGui::Button("Normals");
		if (ImGui::Button("Wireframe"))
		{
			renderer.toggleWireFrame();
		}
	}
	ImGui::End();
}
