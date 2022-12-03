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

		if (ImGui::SliderFloat("Near plane dist", &nearplane, 0.1f, 1000.0f))
			App->camera->SetNearPlane(nearplane);

		if (ImGui::SliderFloat("Far plane dist", &farplane, 0.1f, 2000.0f))
			App->camera->SetFarPlane(farplane);
		ImGui::Separator();
	}

	if (ImGui::CollapsingHeader("Textures"))
	{
		
	}

	this->setFocused(ImGui::IsWindowFocused());

	ImGui::End();
}