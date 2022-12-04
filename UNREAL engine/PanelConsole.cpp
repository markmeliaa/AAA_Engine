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

	colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1, 1, 1, 1);

	for (int i = 0; i < App->editor->log.size(); ++i)
	{
		if (App->editor->log[i][0] == 42) // * = 42
			ImGui::TextColored(ImVec4(0.55f, 0.55f, 0.55f, 1.0f), App->editor->log[i]);
		else if (App->editor->log[i][0] == 45) // - = 45
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), App->editor->log[i]);
		else
			ImGui::TextUnformatted(App->editor->log[i]);

		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);
	}

	this->setFocused(ImGui::IsWindowFocused());

	colors[ImGuiCol_Text] = ImVec4(0, 0, 0, 1);

	ImGui::End();
}