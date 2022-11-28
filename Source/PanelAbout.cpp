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

	ImGui::Text(TITLE);
	ImGui::Text("- You get the name? This is not (yet) a real engine");
	ImGui::Text("");
	ImGui::Text("Developed by Mark Meliaa.");
	ImGui::Separator();

	ImGui::Text("So far, the libraries being used are:");
	ImGui::Text("	- SDL");
	ImGui::Text("	- GLEW");
	ImGui::Text("	- MathGeoLib");
	ImGui::Text("	- ImGui");
	ImGui::Text("	- DebugDraw");
	ImGui::Text("	- DirectXTex");
	ImGui::Text("	- Assimp");
	ImGui::Separator();

	ImGui::Text("Copyright (c) 2022 Marc Alcon Melia");
	ImGui::Separator();

	this->setFocused(ImGui::IsWindowFocused());

	ImGui::End();
}