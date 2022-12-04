#pragma once
#include "PanelAbout.h"

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include <GL/glew.h>
#include <string>

PanelAbout::PanelAbout()
{
}

PanelAbout::~PanelAbout()
{
}

void PanelAbout::Draw()
{
	if (!this->visible)
	{
		this->setFocused(false);
		return;
	}

	ImGui::SetNextWindowSize(ImVec2(385, 265), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(0, 18), ImGuiCond_Always);
	ImGui::Begin("About...", &visible);

	colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1, 1, 1, 1);

	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), TITLE);
	ImGui::Text("- Get the name? This is not (yet) a real engine");
	ImGui::Text("");
	ImGui::Text("Developed by Mark Meliaa.");
	ImGui::Separator();

	ImGui::Text("So far, the libraries being used are:");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "	- SDL");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "	- GLEW");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "	- MathGeoLib");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "	- ImGui");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "	- DebugDraw");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "	- DirectXTex");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "	- Assimp");
	ImGui::Separator();

	ImGui::Text("Copyright (c) 2022 Marc Alcon Melia");
	ImGui::Separator();

	this->setFocused(ImGui::IsWindowFocused());

	colors[ImGuiCol_Text] = ImVec4(0, 0, 0, 1);

	ImGui::End();
}