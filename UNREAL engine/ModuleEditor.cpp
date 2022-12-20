#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"

#include "PanelAbout.h"
#include "PanelConsole.h"
#include "PanelConfig.h"
#include "PanelProperties.h"

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

using namespace std;

ModuleEditor::ModuleEditor()
{
	panels.push_back(props = new PanelProperties());
	panels.push_back(config = new PanelConfig());
	panels.push_back(console = new PanelConsole());
	panels.push_back(about = new PanelAbout());
}

ModuleEditor::~ModuleEditor()
{
	for (int i = 0; i < panels.size(); ++i)
	{
		delete panels[i];
	}

	log = {};
}

bool ModuleEditor::Init()
{
	//D_LOG("Create ImGui context");
	log.emplace_back("Create ImGui context");
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	return true;
}

bool ModuleEditor::Start()
{
	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->GetContext());
	ImGui_ImplOpenGL3_Init(GLSL_VERSION);
	return true;
}

update_status ModuleEditor::PreUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update()
{
	SetUpEditorColors();
	update_status ret = DrawMainMenu();

	for (int i = 0; i < panels.size(); ++i)
		panels[i]->Draw();

	return ret;
}

update_status ModuleEditor::PostUpdate()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	SDL_GL_MakeCurrent(App->window->window, App->renderer->GetContext());

	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	bool ret = true;

	for (int i = 0; i < panels.size(); ++i)
		ret = panels[i]->CleanUp();

	return ret;
}

bool ModuleEditor::IsAnyWindowsFocused()
{
	for (int i = 0; i < panels.size(); ++i)
	{
		if (panels[i]->getFocused())
			return true;
	}

	return false;
}

update_status ModuleEditor::DrawMainMenu()
{
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1, 1, 1, 1);

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit Engine (Esc)"))
				return UPDATE_STOP;

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("About the Engine", NULL, &about->visible))
				about->visible = true;

			if (ImGui::MenuItem("Console Log", NULL, &console->visible))
				console->visible = true;

			if (ImGui::MenuItem("Configuration", NULL, &config->visible))
				config->visible = true;

			if (ImGui::MenuItem("Model Properties", NULL, &props->visible))
				props->visible = true;

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Github"))
		{
			if (ImGui::MenuItem("Documentation"))
				App->RequestBrowser("https://github.com/markmeliaa/AAA_Engine/wiki");

			if (ImGui::MenuItem("Download Latest"))
				App->RequestBrowser("https://github.com/markmeliaa/AAA_Engine/releases");

			if (ImGui::MenuItem("Report a Bug"))
				App->RequestBrowser("https://github.com/markmeliaa/AAA_Engine/issues");

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	colors[ImGuiCol_Text] = ImVec4(0, 0, 0, 1);

	return UPDATE_CONTINUE;
}

void ModuleEditor::SetUpEditorColors()
{
	ImVec4* colors = ImGui::GetStyle().Colors;

	colors[ImGuiCol_TitleBg] = ImVec4(0.75f, 0.75f, 0.75f, 1.0f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.95f, 0.65f, 0.0f, 1.0f);

	colors[ImGuiCol_Header] = ImVec4(0.95f, 0.80f, 0.0f, 1.0f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.95f, 0.75f, 0.0f, 1.0f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.95f, 0.75f, 0.0f, 1.0f);

	colors[ImGuiCol_Button] = ImVec4(0.95f, 0.80f, 0.0f, 1.0f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.95f, 0.75f, 0.0f, 1.0f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.95f, 0.70f, 0.0f, 1.0f);

	colors[ImGuiCol_CheckMark] = ImVec4(0.95f, 0.80f, 0.0f, 1.0f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.95f, 0.80f, 0.0f, 1.0f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.95f, 0.75f, 0.0f, 1.0f);

	colors[ImGuiCol_ResizeGrip] = ImVec4(0.95f, 0.80f, 0.0f, 0.2f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.95f, 0.80f, 0.0f, 1.0f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.95f, 0.80f, 0.0f, 1.0f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.95f, 0.80f, 0.0f, 1.0f);
	colors[ImGuiCol_Separator] = ImVec4(0.95f, 0.80f, 0.0f, 1.0f);

	colors[ImGuiCol_Text] = ImVec4(0, 0, 0, 1);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.95f, 0.75f, 0.0f, 1.0f);
}