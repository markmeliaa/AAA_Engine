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

	ImGui::SetNextWindowSize(ImVec2(400, (float)App->window->GetCurrentHeight() / 2), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2((float)App->window->GetCurrentWidth() - 400, 18), ImGuiCond_Always);
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::Begin("Configuration", &visible);

	colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(0, 0, 0, 1);

	if (ImGui::CollapsingHeader("Application"))
	{
		colors[ImGuiCol_Text] = ImVec4(1, 1, 1, 1);

		static char eng_name[128] = "\"UNREAL\" engine";
		ImGui::InputText("Engine name", eng_name, IM_ARRAYSIZE(eng_name));

		static char org_name[128] = "UPC Tech Talent";
		ImGui::InputText("Organization", org_name, IM_ARRAYSIZE(org_name));

		ImGui::PlotHistogram("##framerate", App->editor->fps_log, IM_ARRAYSIZE(App->editor->fps_log), 0, "Framerate", 0.0f, 100.0f, ImVec2(385.0f, 100.0f));
		ImGui::PlotHistogram("##milliseconds", App->editor->milisec_log, IM_ARRAYSIZE(App->editor->milisec_log), 0, "Milliseconds", 0.0f, 40.0f, ImVec2(385.0f, 100.0f));
		ImGui::Separator();

		colors[ImGuiCol_Text] = ImVec4(0, 0, 0, 1);
	}

	if (ImGui::CollapsingHeader("Renderer"))
	{
		colors[ImGuiCol_Text] = ImVec4(1, 1, 1, 1);

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

		colors[ImGuiCol_Text] = ImVec4(0, 0, 0, 1);
	}

	if (ImGui::CollapsingHeader("Window"))
	{
		colors[ImGuiCol_Text] = ImVec4(1, 1, 1, 1);

		static bool win_fullscreen = FULLSCREEN;
		static bool win_resizable = RESIZABLE;
		static bool win_borderless = BORDERLESS;
		static bool win_fullscreen_dsktp = FULLSCREEN_DSKTP;

		static bool changing_screen_size_w = false;
		static bool changing_screen_size_h = false;

		static int static_width = 0;
		static int static_height = 0;

		static float bright = App->window->GetWindowBrightness();
		if (ImGui::SliderFloat("Brightness", &bright, 0.0f, 1.0f))
			App->window->SetWindowBrightness(bright);

		int width = App->window->GetCurrentWidth();
		if (ImGui::SliderInt("Screen Width", &width, 820, App->window->GetMaxWindowsWidth()))
		{
			changing_screen_size_w = true;
			static_width = width;
		}

		if (!ImGui::IsAnyMouseDown() && changing_screen_size_w)
		{
			App->window->SetCurrentWidth(static_width);

			if (static_width < App->window->GetMaxWindowsWidth())
			{
				win_fullscreen_dsktp = false;
			}

			App->window->SetWindowSize(App->window->GetCurrentWidth(), App->window->GetCurrentHeight());

			changing_screen_size_w = false;
		}

		int height = App->window->GetCurrentHeight();
		if (ImGui::SliderInt("Screen Height", &height, 685, App->window->GetMaxWindowsHeight()))
		{
			changing_screen_size_h = true;
			static_height = height;
		}

		if (!ImGui::IsAnyMouseDown() && changing_screen_size_h)
		{
			App->window->SetCurrentHeight(static_height);

			if (static_height < App->window->GetMaxWindowsHeight())
			{
				win_fullscreen_dsktp = false;
			}

			else if (static_width == App->window->GetMaxWindowsWidth() && static_height == App->window->GetMaxWindowsHeight() && !win_fullscreen)
			{
				if (!win_fullscreen_dsktp)
					win_fullscreen_dsktp = true;
			}

			App->window->SetWindowSize(App->window->GetCurrentWidth(), App->window->GetCurrentHeight());

			changing_screen_size_h = false;
		}

		ImGui::Text("Refresh rate: ");
		ImGui::SameLine();
		std::string s = std::to_string((int)ImGui::GetIO().Framerate);
		char const* framerate = s.c_str();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), framerate);

		if (ImGui::Checkbox("FULLSCREEN     ", &win_fullscreen))
		{
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
				width = App->window->GetMaxWindowsWidth();
				height = App->window->GetMaxWindowsHeight();
				App->window->SetWindowSize(App->window->GetMaxWindowsWidth(), App->window->GetMaxWindowsHeight());
			}

			else
			{
				width = BASE_SCREEN_WIDTH;
				height = BASE_SCREEN_HEIGHT;
				App->window->SetWindowSize(BASE_SCREEN_WIDTH, BASE_SCREEN_HEIGHT);
			}
		}

		ImGui::Separator();

		colors[ImGuiCol_Text] = ImVec4(0, 0, 0, 1);
	}

	if (ImGui::CollapsingHeader("Camera"))
	{
		colors[ImGuiCol_Text] = ImVec4(1, 1, 1, 1);

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

		colors[ImGuiCol_Text] = ImVec4(0, 0, 0, 1);
	}

	if (ImGui::CollapsingHeader("Texture"))
	{
		colors[ImGuiCol_Text] = ImVec4(1, 1, 1, 1);

		ImGui::Text("Modify the texture:");

		colors[ImGuiCol_Text] = ImVec4(0, 0, 0, 1);
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

		colors[ImGuiCol_Text] = ImVec4(0, 0, 0, 1);
	}

	this->setFocused(ImGui::IsWindowFocused());

	colors[ImGuiCol_Text] = ImVec4(0, 0, 0, 1);

	ImGui::End();
}