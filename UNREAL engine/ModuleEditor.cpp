#pragma once
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
	for (list<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it)
	{
		delete* it;
	}
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

	CalculateFpsPerFrame();
	CalculateMilisecPerFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update()
{
	update_status ret = DrawMainMenu();

	for (list<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it)
		(*it)->Draw();

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

	for (list<Panel*>::reverse_iterator it = panels.rbegin(); it != panels.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	return ret;
}

void ModuleEditor::CalculateFpsPerFrame()
{
	int last_checked = 0;
	for (int i = 0; i < sizeof(fps_log) / sizeof(float); ++i)
	{
		last_checked = i;
		if (fps_log[last_checked] == NULL)
		{
			fps_log[last_checked] = ImGui::GetIO().Framerate;
			break;
		}

		else if (fps_log[last_checked + 1] == NULL)
		{
			fps_log[last_checked + 1] = ImGui::GetIO().Framerate;
			break;
		}

		else
			fps_log[last_checked] = fps_log[last_checked + 1];
	}

	if (last_checked == sizeof(fps_log) / sizeof(float) - 1)
		fps_log[last_checked] = ImGui::GetIO().Framerate;
}

void ModuleEditor::CalculateMilisecPerFrame()
{
	int last_checked = 0;
	for (int i = 0; i < sizeof(milisec_log) / sizeof(float); ++i)
	{
		last_checked = i;
		if (milisec_log[last_checked] == NULL)
		{
			milisec_log[last_checked] = ImGui::GetIO().DeltaTime * 1000;
			break;
		}

		else if (milisec_log[last_checked + 1] == NULL)
		{
			milisec_log[last_checked + 1] = ImGui::GetIO().DeltaTime * 1000;
			break;
		}

		else
			milisec_log[last_checked] = milisec_log[last_checked + 1];
	}

	if (last_checked == sizeof(milisec_log) / sizeof(float) - 1)
		milisec_log[last_checked] = ImGui::GetIO().DeltaTime * 1000;
}

bool ModuleEditor::IsAnyWindowsFocused()
{
	for (list<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it)
	{
		if ((*it)->getFocused())
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