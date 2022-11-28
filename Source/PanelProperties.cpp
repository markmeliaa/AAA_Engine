#include "Application.h"
#include "PanelProperties.h"
#include "ModuleWindow.h"
#include "ModuleEditor.h"
#include "ModuleTexture.h"
#include "ModuleRender.h"

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include <string>

PanelProperties::PanelProperties()
{
}

PanelProperties::~PanelProperties()
{
}

void PanelProperties::Draw()
{
	if (!this->visible)
	{
		this->setFocused(false);
		return;
	}

	ImGui::SetNextWindowSize(ImVec2(400, (float)App->window->getCurrentHeight() / 2), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2((float)App->window->getCurrentWidth() - 400, (float)App->window->getCurrentHeight() / 2 + 18), ImGuiCond_Always);
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::Begin("Properties", &visible);

	if (ImGui::CollapsingHeader("Transformation"))
	{
		ImGui::Text("Nothing yet...");
		ImGui::Separator();
	}

	if (ImGui::CollapsingHeader("Geometry"))
	{
		ImGui::TextUnformatted("AMOUNT OF VERTICES:");
		for (int i = 0; i < App->renderer->GetModel()->GetMeshes().size(); i++)
		{
			ImGui::TextUnformatted("Mesh");
			ImGui::SameLine();
			ImGui::TextUnformatted(std::to_string(i).c_str());
			ImGui::SameLine();
			ImGui::TextUnformatted(":");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), 
				std::to_string(App->renderer->GetModel()->GetMeshes()[i]->GetNumVertices()).c_str());
		}
		ImGui::Separator();

		ImGui::TextUnformatted("AMOUNT OF TRIANGLES:");
		for (int i = 0; i < App->renderer->GetModel()->GetMeshes().size(); i++)
		{
			ImGui::TextUnformatted("Mesh");
			ImGui::SameLine();
			ImGui::TextUnformatted(std::to_string(i).c_str());
			ImGui::SameLine();
			ImGui::TextUnformatted(":");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f),
				std::to_string(App->renderer->GetModel()->GetMeshes()[i]->GetNumIndices() / 3).c_str());
		}
	}

	if (ImGui::CollapsingHeader("Texture"))
	{
		ImGui::TextUnformatted("Size of the texture:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), std::to_string(App->texture->GetImageMetadata().width).c_str());
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "x");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), std::to_string(App->texture->GetImageMetadata().height).c_str());
		ImGui::Separator();
	}

	this->setFocused(ImGui::IsWindowFocused());

	ImGui::End();
}