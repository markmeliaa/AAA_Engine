#include "Application.h"
#include "PanelConfig.h"
#include "ModuleWindow.h"
#include "ModuleEditor.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include <string>
#include <SDL.h>
#include <GL/glew.h>
#include <assimp/version.h>

PanelConfig::PanelConfig()
{
}

PanelConfig::~PanelConfig()
{
}

void PanelConfig::Draw()
{
	if (!this->visible)
	{
		this->setFocused(false);
		return;
	}

	ImGui::SetNextWindowSize(ImVec2(400, (float)App->window->getCurrentHeight() / 2), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2((float)App->window->getCurrentWidth() - 400, 18), ImGuiCond_Always);
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::Begin("Configuration", &visible);

	if (ImGui::CollapsingHeader("Application"))
	{
		static char eng_name[128] = "\"UNREAL\" engine";
		ImGui::InputText("Engine name", eng_name, IM_ARRAYSIZE(eng_name));

		static char org_name[128] = "UPC Tech Talent";
		ImGui::InputText("Organization", org_name, IM_ARRAYSIZE(org_name));

		static int maax_fps = App->editor->GetMaxFps();
		if (ImGui::SliderInt("Max FPS", &maax_fps, 0, 60))
			App->editor->SetMaxFps(maax_fps);

		ImGui::Text("Limit framerate:");
		ImGui::SameLine();
		std::string s = std::to_string((int)App->editor->GetMaxFps());
		char const* char_fps = s.c_str();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), char_fps);

		ImGui::PlotHistogram("##framerate", App->editor->fps_log, IM_ARRAYSIZE(App->editor->fps_log), 0, "Framerate", 0.0f, 100.0f, ImVec2(385.0f, 100.0f));
		ImGui::PlotHistogram("##milliseconds", App->editor->milisec_log, IM_ARRAYSIZE(App->editor->milisec_log), 0, "Milliseconds", 0.0f, 40.0f, ImVec2(385.0f, 100.0f));
		ImGui::Separator();
	}

	if (ImGui::CollapsingHeader("Renderer"))
	{
		float background[3] = { App->renderer->background_color[0], App->renderer->background_color[1], App->renderer->background_color[2] };

		if (ImGui::ColorEdit3("Background color", background))
		{
			App->renderer->background_color[0] = background[0];
			App->renderer->background_color[1] = background[1];
			App->renderer->background_color[2] = background[2];
		}
		ImGui::Separator();
	}

	if (ImGui::CollapsingHeader("Window"))
	{
		static bool win_fullscreen = false;
		static bool win_resizable = false;
		static bool win_borderless = false;
		static bool win_fullscreen_dsktp = false;

		static float bright = App->window->GetWindowBrightness();
		if (ImGui::SliderFloat("Brightness", &bright, 0.0f, 1.0f))
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
		ImGui::Separator();
	}

	if (ImGui::CollapsingHeader("Camera"))
	{
		ImGui::Text("Camera speed:");
		static float movespeed = App->camera->GetMoveSpeed();
		static float rotspeed = App->camera->GetRotSpeed();
		static float zoomspeed = App->camera->GetZoomSpeed();

		if (ImGui::SliderFloat("Move speed", &movespeed, 1.0f, 10.0f))
			App->camera->SetMoveSpeed(movespeed);

		if (ImGui::SliderFloat("Rotation speed", &rotspeed, 1.0f, 50.0f))
			App->camera->SetRotSpeed(rotspeed);

		if (ImGui::SliderFloat("Zoom speed", &zoomspeed, 1.0f, 10.0f))
			App->camera->SetZoomSpeed(zoomspeed);
		ImGui::Separator();

		ImGui::Text("Frustum planes:");
		static float nearplane = App->camera->GetNearPlane();
		static float farplane = App->camera->GetFarPlane();

		if (ImGui::SliderFloat("Near plane dist", &nearplane, 0.1f, 100.0f))
			App->camera->SetNearPlane(nearplane);

		if (ImGui::SliderFloat("Far plane dist", &farplane, 0.1f, 100.0f))
			App->camera->SetFarPlane(farplane);
		ImGui::Separator();
	}

	if (ImGui::CollapsingHeader("Hardware"))
	{
		SDL_version sdl_ver = SDL_version{};
		SDL_VERSION(&sdl_ver);

		ImGui::Text("SDL version:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%u.%u.%u", sdl_ver.major, sdl_ver.minor, sdl_ver.patch);

		ImGui::Text("GLEW version:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i.%i.%i", GLEW_VERSION_MAJOR, GLEW_VERSION_MINOR, GLEW_VERSION_MICRO);

		ImGui::Text("GLSL version:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", glGetString(GL_SHADING_LANGUAGE_VERSION));

		ImGui::Text("Assimp version:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i.%i.%i", aiGetVersionMajor(), aiGetVersionMinor(), aiGetVersionRevision());
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

		ImGui::Text("GPU:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", glGetString(GL_RENDERER));

		ImGui::Text("Brand:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", glGetString(GL_VENDOR));

		ImGui::Text("VRAM Size:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "-----");

		ImGui::Text("VRAM Available:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "-----");

		ImGui::Text("VRAM Usage:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "-----");
		ImGui::Separator();
	}

	this->setFocused(ImGui::IsWindowFocused());

	ImGui::End();
}