#include "Application.h"
#include "PanelConsole.h"
#include "ModuleWindow.h"
#include "ModuleEditor.h"

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

PanelConsole::PanelConsole()
{
}

PanelConsole::~PanelConsole()
{
}

void PanelConsole::Draw()
{
	if (!visible)
	{
		this->setFocused(false);
		return;
	}

	ImGui::SetNextWindowSize(ImVec2((float)App->window->getCurrentWidth()/2, 200), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(0, (float)App->window->getCurrentHeight() - 200), ImGuiCond_Always);
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::Begin("Console Log", &visible);

	for (int i = 0; i < App->editor->log.size(); i++)
	{
		ImGui::TextUnformatted(App->editor->log[i]);
		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);
	}

	this->setFocused(ImGui::IsWindowFocused());

	ImGui::End();
}