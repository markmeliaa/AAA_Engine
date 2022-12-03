#include "Application.h"
#include "PanelConfig.h"
#include "ModuleWindow.h"
#include "ModuleEditor.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"

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
		ImGui::TextUnformatted("Change the color of the background:");

		float background[3] = { App->renderer->background_color[0], App->renderer->background_color[1], App->renderer->background_color[2] };
		ImGui::SetNextItemWidth(195);
		if (ImGui::ColorPicker3("##MyColor##1", (float*)&background, ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha))
		{
			App->renderer->background_color[0] = background[0];
			App->renderer->background_color[1] = background[1];
			App->renderer->background_color[2] = background[2];
		}
		ImGui::SameLine();
		ImGui::SetNextItemWidth(195);
		if (ImGui::ColorPicker3("##MyColor##2", (float*)&background, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha))
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

		static bool changing_screen_size_w = false;
		static bool changing_screen_size_h = false;

		static int static_width = 0;
		static int static_height = 0;

		static float bright = App->window->GetWindowBrightness();
		if (ImGui::SliderFloat("Brightness", &bright, 0.0f, 1.0f))
			App->window->SetWindowBrightness(bright);

		int width = App->window->getCurrentWidth();
		if (ImGui::SliderInt("Screen Width", &width, 820, App->window->getMaxWindowsWidth()))
		{
			changing_screen_size_w = true;
			static_width = width;
		}

		if (!ImGui::IsAnyMouseDown() && changing_screen_size_w)
		{
			App->window->setCurrentWidth(static_width);

			if (static_width < App->window->getMaxWindowsWidth())
			{
				win_fullscreen_dsktp = false;
			}

			App->window->SetWindowSize(App->window->getCurrentWidth(), App->window->getCurrentHeight());

			changing_screen_size_w = false;
		}

		int height = App->window->getCurrentHeight();
		if (ImGui::SliderInt("Screen Height", &height, 685, App->window->getMaxWindowsHeight()))
		{
			changing_screen_size_h = true;
			static_height = height;
		}

		if (!ImGui::IsAnyMouseDown() && changing_screen_size_h)
		{
			App->window->setCurrentHeight(static_height);

			if (static_height < App->window->getMaxWindowsHeight())
			{
				win_fullscreen_dsktp = false;
			}

			else if (static_width == App->window->getMaxWindowsWidth() && static_height == App->window->getMaxWindowsHeight() && !win_fullscreen)
			{
				if (!win_fullscreen_dsktp)
					win_fullscreen_dsktp = true;
			}

			App->window->SetWindowSize(App->window->getCurrentWidth(), App->window->getCurrentHeight());

			changing_screen_size_h = false;
		}

		ImGui::Text("Refresh rate: ");
		ImGui::SameLine();
		std::string s = std::to_string((int)ImGui::GetIO().Framerate);
		char const* framerate = s.c_str();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), framerate);

		if (ImGui::Checkbox("FULLSCREEN     ", &win_fullscreen))
		{
			if (win_fullscreen)
			{
				App->window->setCurrentWidth(App->window->getMaxWindowsWidth());
				width = App->window->getMaxWindowsWidth();
				App->window->setCurrentHeight(App->window->getMaxWindowsHeight());
				height = App->window->getMaxWindowsHeight();

				App->window->SetWindowSize(App->window->getCurrentWidth(), App->window->getCurrentHeight());
			}

			App->window->SetWindowFullscreen(win_fullscreen);
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

	if (ImGui::CollapsingHeader("Texture"))
	{
		ImGui::Text("Modify the texture:");
		if (ImGui::Button("Change WRAP mode", ImVec2(121.5f, 25.0f)))
		{
			App->texture->SetWrapMode();
			App->texture->SetTextureOptions();
		}
		ImGui::SameLine();
		if (ImGui::Button("Change MAG filt.", ImVec2(121.5f, 25.0f)))
		{
			App->texture->SetMagFilter();
			App->texture->SetTextureOptions();
		}
		ImGui::SameLine();
		if (ImGui::Button("Change MIN filt.", ImVec2(121.5f, 25.0f)))
		{
			App->texture->SetMinFilter();
			App->texture->SetTextureOptions();
		}
		ImGui::Separator();
	}

	this->setFocused(ImGui::IsWindowFocused());

	ImGui::ShowDemoWindow();

	ImGui::End();
}