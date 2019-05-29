#include "engine.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"

#include "input.hpp"

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
	tools.push_back(new ToolExpand());
	tools.push_back(new ToolSmoothen());
	tools.push_back(new ToolCurve());
	for (auto tool : tools)
	{
		tool->init();
	}

	glm::vec3 clearColor{ 0.7f };
	glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0);

	cube.loadTexture("assets/output-onlinepngtools.png");
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
	showCameraSettings();
	//ImGui::ShowDemoWindow();
	if (currentTool && currentTool->hasSpecialGUI())
	{
		currentTool->showSpecialGUI();
	}


	ImGui::EndFrame();


	
	if (canUseTool())
	{
		float radius = glm::pow(brushSettings.radius*0.1f, 1.5f) * 0.2f;
		float strength = brushSettings.strength * 0.5f * dt * radius;

		float normStr = (brushSettings.strength-1.f) / (brushSettings.maxStrength-1.f);

		renderer.showBrush(radius, normStr);

		if (!brushCrossActive)
		{
			GLuint uvSSBO = renderer.mouseTerrainIntersection();

			if (Input::isKeyDown(GLFW_KEY_LEFT_CONTROL))
			{
				strength *= -1.f;
			}

			if (Input::isMouseButtonDown(GLFW_MOUSE_BUTTON_1) && currentTool)
			{
				currentTool->setVectorMap(&vmap);
				currentTool->use(uvSSBO, radius, strength);
			}
		}
	}

	if (Input::isKeyDown(GLFW_KEY_LEFT_ALT))
	{
		if (!brushCrossActive)
		{
			brushCrossActive = true;

			glm::vec2 ws = Window::size();
			glm::vec2 mp = Input::mousePosition();

			brushCrossNDC = 2.f*mp / ws - 1.f;
			brushCrossNDC.y = -brushCrossNDC.y;
		}

		glm::vec2 mm = Input::mouseMovement();

		brushSettings.strength += 0.05f*mm.x;
		brushSettings.radius -= 0.05f*mm.y;

		brushSettings.strength = glm::clamp(brushSettings.strength, 1.f, 10.f);
		brushSettings.radius = glm::clamp(brushSettings.radius, 1.f, 10.f);

		renderer.showBrushCross(brushCrossNDC);
	}
	else
	{
		brushCrossActive = false;
	}
}

void Engine::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	minimap.bindFBO();
	renderer.renderMiniMap(minimap.getTransform(), minimap.getPosition());
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
		if (ImGui::BeginMenu("Camera"))
		{
			if(ImGui::MenuItem("Show Settings", "", displayCameraSettings))
			{
				displayCameraSettings = (displayCameraSettings == false) ? true : false;
			}
			if (ImGui::MenuItem("Show Orientation Menu", "", displayOrientationMenu))
			{
				displayOrientationMenu = (displayOrientationMenu == false) ? true : false;
			}
			if (ImGui::MenuItem("Show Minimap", "", displayMinimap))
			{
				displayMinimap = (displayMinimap == false) ? true : false;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void Engine::showMiniMap()
{
	if (displayMinimap)
	{
		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoCollapse;
		window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
		ImTextureID tex = reinterpret_cast<ImTextureID>(minimap.getTextureID());
		if (ImGui::Begin("Minimap", 0, window_flags))
		{
			ImGui::Image(tex, ImVec2(300, 300), ImVec2(0,1), ImVec2(1,0));
		}
		ImGui::End();
	}
}
void Engine::showCameraSettings()
{
	if (displayCameraSettings)
	{
		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoCollapse;
		window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
		if (ImGui::Begin("Camera Settings", 0, window_flags))
		{
			if (ImGui::Button("Change Camera"))
			{
				renderer.toggleFPSCamera();
			}
		}
		ImGui::End();
	}
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
				if (i == 0 && renderer.ViewModeIsAuto()) {
					renderer.setViewMode(VIEWMODE_SHADED);
				}
				if (i == 1 && renderer.ViewModeIsAuto()) {
					renderer.setViewMode(VIEWMODE_NORMALS);
				}
			}
			ImGui::PopID();
		}

		ImGui::Separator();
		std::string toolNameText = "Selected: ";
		if (currentTool)
		{
			toolNameText += currentTool->name();
		}
		else
		{
			toolNameText += "None";
		}
		ImGui::Text(toolNameText.c_str());
		ImGui::PushItemWidth(100);

		ImGui::SliderFloat("Size", &brushSettings.radius, 1, brushSettings.maxRadius, "%.f");
		ImGui::SliderFloat("Strength", &brushSettings.strength, 1, brushSettings.maxStrength, "%.f");
	}
	ImGui::End();
}

void Engine::showOrientationMenu()
{
	if (displayOrientationMenu)
	{
		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoCollapse;
		window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
		window_flags |= ImGuiWindowFlags_NoMove;
		/*if (ImGui::Begin("Orientation", 0, window_flags))
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
		ImGui::End();*/
		if (ImGui::Begin("Orientation", 0, window_flags))
		{
			ImTextureID tex = reinterpret_cast<ImTextureID>(cube.getID());
			ImGui::Image(tex, ImVec2(150, 150));
		} ImGui::End();
	}
}

void Engine::showShadingMenu()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
	
	if (ImGui::Begin("Shading", 0, window_flags))
	{
		//std::string viewNameText = "Selected: Shaded";
		ImGui::PushItemWidth(300);
		if (ImGui::Button("Shaded")) {
			renderer.setViewMode(VIEWMODE_SHADED);
			renderer.setViewModeAutomatic(false);
			viewNameText = "Shaded";
		}
		if (ImGui::Button("Textured")) {
			renderer.setViewMode(VIEWMODE_TEXTURED);
			renderer.setViewModeAutomatic(false);
			viewNameText = "Textured";
		}
		if (ImGui::Button("Normals")) {
			renderer.setViewMode(VIEWMODE_NORMALS);
			renderer.setViewModeAutomatic(false);
			viewNameText = "Normals";
		}
		if (ImGui::Button("Wireframe"))
		{
			//renderer.toggleWireFrame();
			renderer.setViewMode(VIEWMODE_WIREFRAME);
			renderer.setViewModeAutomatic(false);
			viewNameText = "Wireframe";
		}
		if (ImGui::Button("Auto"))
		{
			renderer.setViewModeAutomatic(true);
			viewNameText = "Auto";
		}
		ImGui::Text(viewNameText.c_str());
		
	}
	ImGui::End();
}
