#pragma once
#pragma warning( disable : 4244 )
#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ModuleInput.h"

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

ModuleEditor::ModuleEditor()
{
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
	static bool demo_w = false;
	static bool log_w = false;
	static bool config_w = false;
	static bool about_w = false;

	DrawMainMenu(demo_w);

	DrawLog(log_w);
	DrawConfig(config_w);
	DrawAbout(about_w);

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

void ModuleEditor::DrawMainMenu(bool& demo_w) const
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

		if (ImGui::BeginMenu("Help"))
		{
			/*
			if (ImGui::MenuItem("ImGui Demo"))
			{
				ImGui::ShowDemoWindow(&demo_w);
				demo_w = !demo_w;
			}
			*/

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

void ModuleEditor::DrawLog(bool& log_w)
{
	ImGui::SetNextWindowSize(ImVec2(550, 200), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(0, App->window->getCurrentHeight() - 200), ImGuiCond_Always);
	//ImGui::SetNextWindowBgAlpha(0.75f);
	ImGui::Begin("Console Log", &log_w);

	for (int i = 0; i < log.size(); i++)
	{
		ImGui::TextUnformatted(log[i]);
		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);
	}

	if (ImGui::IsWindowFocused())
		WindowsFocused[0] = true;
	else
		WindowsFocused[0] = false;

	ImGui::End();
}

void ModuleEditor::DrawAbout(bool& about_w)
{
	ImGui::SetNextWindowSize(ImVec2(385, 265), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(0, 18), ImGuiCond_Always);
	//ImGui::SetNextWindowBgAlpha(0.75f);
	ImGui::Begin("About...", &about_w);

	ImGui::Text(TITLE);
	ImGui::Text("- You get the name? This is not (yet) a real engine");
	ImGui::Text("");
	ImGui::Text("Developed by Mark Meliaa.");
	ImGui::Separator();

	ImGui::Text("So far, the libraries being used are:");
	ImGui::Text("	- SDL");
	ImGui::Text("	- GLEW");
	ImGui::Text("	- MathLib");
	ImGui::Text("	- ImGui");
	ImGui::Text("	- DebugDraw");
	ImGui::Text("	- DirectXTex");
	ImGui::Text("	- Assimp");
	ImGui::Separator();

	ImGui::Text("Copyright (c) 2022 Marc Alcon Melia");
	ImGui::Separator();

	if (ImGui::IsWindowFocused())
		WindowsFocused[1] = true;
	else
		WindowsFocused[1] = false;

	ImGui::End();
}

void ModuleEditor::DrawConfig(bool& config_w)
{
	ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(App->window->getCurrentWidth() - 400, 18), ImGuiCond_Always);
	//ImGui::SetNextWindowBgAlpha(0.75f);
	ImGui::Begin("Configuration", &config_w);

	if (ImGui::BeginMenu("Options"))
	{
		if (ImGui::MenuItem("TBD 1"))
		{

		}

		if (ImGui::MenuItem("TBD 2"))
		{

		}

		ImGui::EndMenu();
	}

	if (ImGui::CollapsingHeader("Application"))
	{
		static char eng_name[128] = "\"UNREAL\" engine";
		ImGui::InputText("Engine name", eng_name, IM_ARRAYSIZE(eng_name));

		static char org_name[128] = "UPC Tech Talent";
		ImGui::InputText("Organization", org_name, IM_ARRAYSIZE(org_name));

		static int maax_fps = GetMaxFps();
		ImGui::SliderInt("Max FPS", &maax_fps, 0, 60);
		SetMaxFps(maax_fps);

		ImGui::Text("Limit framerate:");
		ImGui::SameLine();
		std::string s = std::to_string((int)GetMaxFps());
		char const* char_fps = s.c_str();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), char_fps);


		ImGui::PlotHistogram("##framerate", fps_log, IM_ARRAYSIZE(fps_log), 0, "Framerate", 0.0f, 100.0f, ImVec2(385.0f, 100.0f));
		ImGui::PlotHistogram("##milliseconds", milisec_log, IM_ARRAYSIZE(milisec_log), 0, "Milliseconds", 0.0f, 40.0f, ImVec2(385.0f, 100.0f));
	}

	if (ImGui::CollapsingHeader("Window"))
	{
		static bool win_fullscreen = false;
		static bool win_resizable = false;
		static bool win_borderless = false;
		static bool win_fullscreen_dsktp = false;

		static float bright = App->window->GetWindowBrightness();
		ImGui::SliderFloat("Brightness", &bright, 0.0f, 1.0f);
		App->window->SetWindowBrightness(bright);

		static int width = App->window->getCurrentWidth();
		ImGui::SliderInt("Screen Width", &width, 0, App->window->getMaxWindowsWidth());

		if (!ImGui::IsAnyMouseDown())
		{
			if (width < 820)
				width = 820;

			App->window->setCurrentWidth(width);

			if (width < App->window->getMaxWindowsWidth())
			{
				win_fullscreen = false;
				win_fullscreen_dsktp = false;
			}
		}

		static int height = App->window->getCurrentHeight();
		ImGui::SliderInt("Screen Height", &height, 0, App->window->getMaxWindowsHeight());

		if (!ImGui::IsAnyMouseDown())
		{
			if (height < 685)
				height = 685;
			App->window->setCurrentHeight(height);

			if (height < App->window->getMaxWindowsHeight())
			{
				win_fullscreen = false;
				win_fullscreen_dsktp = false;
			}

			else if (width == App->window->getMaxWindowsWidth() && height == App->window->getMaxWindowsHeight())
			{
				if (!win_fullscreen && !win_fullscreen_dsktp)
					win_fullscreen = true;
			}
		}

		App->window->SetWindowSize(App->window->getCurrentWidth(), App->window->getCurrentHeight());

		ImGui::Text("Refresh rate: ");
		ImGui::SameLine();
		std::string s = std::to_string((int)ImGui::GetIO().Framerate);
		char const* framerate = s.c_str();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), framerate);

		if (ImGui::Checkbox("FULLSCREEN     ", &win_fullscreen))
		{
			if (win_fullscreen)
			{
				width = App->window->getMaxWindowsWidth();
				height = App->window->getMaxWindowsHeight();
				App->window->SetWindowSize(App->window->getMaxWindowsWidth(), App->window->getMaxWindowsHeight());
			}

			else
			{
				width = BASE_SCREEN_WIDTH;
				height = BASE_SCREEN_HEIGHT;
				App->window->SetWindowSize(BASE_SCREEN_WIDTH, BASE_SCREEN_HEIGHT);
			}
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("RESIZABLE", &win_resizable))
		{
			App->window->SetWindowResizable(win_resizable);
		}

		if (ImGui::Checkbox("BORDERLESS     ", &win_borderless))
		{
			App->window->SetWindowBorderless(win_borderless);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("FULLSCREEN DSKTP", &win_fullscreen_dsktp))
		{
			if (win_fullscreen_dsktp)
			{
				width = App->window->getMaxWindowsWidth();
				height = App->window->getMaxWindowsHeight();
				App->window->SetWindowSize(App->window->getMaxWindowsWidth(), App->window->getMaxWindowsHeight());
			}

			else
			{
				width = BASE_SCREEN_WIDTH;
				height = BASE_SCREEN_HEIGHT;
				App->window->SetWindowSize(BASE_SCREEN_WIDTH, BASE_SCREEN_HEIGHT);
			}
		}
	}

	if (ImGui::CollapsingHeader("Hardware"))
	{
		SDL_version sdl_ver;
		SDL_VERSION(&sdl_ver);

		ImGui::Text("SDL version:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%u.%u.%u", sdl_ver.major, sdl_ver.minor, sdl_ver.patch);
		ImGui::Separator();

		ImGui::Text("CPUs:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", SDL_GetCPUCount());
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "(Cache: %dkb)", SDL_GetCPUCacheLineSize());

		ImGui::Text("System RAM:");
		ImGui::SameLine();
		float gb = static_cast<float>(SDL_GetSystemRAM());
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.1fGb", (gb * 0.00104858));

		ImGui::Text("Caps:");
		ImGui::SameLine();
		std::vector<const char*> caps;
		if (SDL_Has3DNow())
			caps.emplace_back("3DNow");

		if (SDL_HasAltiVec())
			caps.emplace_back("AltiVec");

		if (SDL_HasAVX())
			caps.emplace_back("AVX");

		if (SDL_HasAVX2())
			caps.emplace_back("AVX2");

		if (SDL_HasMMX())
			caps.emplace_back("MMX");

		if (SDL_HasRDTSC())
			caps.emplace_back("RDTSC");

		if (SDL_HasSSE())
			caps.emplace_back("SSE");

		if (SDL_HasSSE2())
			caps.emplace_back("SSE2");

		if (SDL_HasSSE3())
			caps.emplace_back("SSE3");

		if (SDL_HasSSE41())
			caps.emplace_back("SSE41");

		if (SDL_HasSSE42())
			caps.emplace_back("SSE42");

		for (int i = 0; i < caps.size(); i++)
		{
			if (i == 5)
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), caps[i]);

			else
			{
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), caps[i]);
				ImGui::SameLine();
			}

		}
		ImGui::Text("");
		ImGui::Separator();
	}

	if (ImGui::CollapsingHeader("Camera Settings"))
	{
		static float cam_fov = App->camera->GetFov();
		ImGui::SliderFloat("Camera FOV", &cam_fov, 0.001f, 3.0f);
		App->camera->SetFov(cam_fov);

		static float aspect_rat = App->camera->GetAspectRatio();
		ImGui::SliderFloat("Aspect Ratio", &aspect_rat, 0.0f, 6.0f);
		App->camera->SetAspectRatio(aspect_rat);
	}

	if (ImGui::IsWindowFocused())
		WindowsFocused[2] = true;
	else
		WindowsFocused[2] = false;

	ImGui::End();
}

void ModuleEditor::SetMaxFps(const float& fps)
{
	max_fps = fps;
}

float ModuleEditor::GetMaxFps() const
{
	return max_fps;
}

bool ModuleEditor::IsAnyWindowsFocused() const
{
	for (int i = 0; i < sizeof(WindowsFocused) / sizeof(bool); i++)
	{
		if (WindowsFocused[i])
			return true;
	}

	return false;
}