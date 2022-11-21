#pragma once
#pragma warning( disable : 4244 )
#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ModuleInput.h"

#include <list>
#include "PanelConsole.h"
#include "PanelAbout.h"
#include "PanelConfig.h"

#include "lib/imgui-docking/imgui.h"
#include "lib/imgui-docking/imgui_impl_sdl.h"
#include "lib/imgui-docking/imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif
#include <string>

using namespace std;

ModuleEditor::ModuleEditor()
{
	panels.push_back(console = new PanelConsole());
	panels.push_back(about = new PanelAbout());
	panels.push_back(config = new PanelConfig());
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init()
{
	//D_LOG("Create ImGui context")
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
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->context);
	ImGui_ImplOpenGL3_Init(GLSL_VERSION);
	return true;
}

update_status ModuleEditor::PreUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	loops++;

	if (loops == 60 || fps_log[sizeof(fps_log)/sizeof(float) - 1] == NULL)
	{
		int last = 0;

		if (ImGui::GetIO().Framerate > max_fps)
			ImGui::GetIO().Framerate = max_fps;

		for (int i = 0; i < sizeof(fps_log) / sizeof(float); i++)
		{
			last = i;
			if (fps_log[i] == NULL)
			{
				fps_log[i] = ImGui::GetIO().Framerate;
				break;
			}

			else if (fps_log[i + 1] == NULL)
			{
				fps_log[i + 1] = ImGui::GetIO().Framerate;
				break;
			}

			else
				fps_log[i] = fps_log[i + 1];
		}

		if (last == sizeof(fps_log) / sizeof(float) - 1)
			fps_log[sizeof(fps_log) / sizeof(float) - 1] = ImGui::GetIO().Framerate;

		last = 0;
		for (int i = 0; i < sizeof(milisec_log) / sizeof(float); i++)
		{
			last = i;
			if (milisec_log[i] == NULL)
			{
				milisec_log[i] = ImGui::GetIO().DeltaTime * 1000;
				break;
			}

			else if (milisec_log[i + 1] == NULL)
			{
				milisec_log[i + 1] = ImGui::GetIO().DeltaTime * 1000;
				break;
			}

			else
				milisec_log[i] = milisec_log[i + 1];
		}

		if (last == sizeof(milisec_log) / sizeof(float) - 1)
			milisec_log[sizeof(milisec_log) / sizeof(float) - 1] = ImGui::GetIO().DeltaTime * 1000;

		loops = 0;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update()
{
	DrawMainMenu();

	for (list<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it)
		(*it)->Draw();

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	SDL_GL_MakeCurrent(App->window->window, App->renderer->context);

	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

void ModuleEditor::DrawMainMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("TBD"))
				App->RequestBrowser("https://github.com/markmeliaa/AAA_Engine");

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("TBD"))
				App->RequestBrowser("https://github.com/markmeliaa/AAA_Engine");

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Console Log", NULL, &console->visible))
				console->visible = true;

			if (ImGui::MenuItem("Configuration", NULL, &config->visible))
				config->visible = true;

			if (ImGui::MenuItem("About the Engine", NULL, &about->visible))
				about->visible = true;

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
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
}

void ModuleEditor::SetMaxFps(const int& fps)
{
	max_fps = fps;
}

int ModuleEditor::GetMaxFps() const
{
	return max_fps;
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