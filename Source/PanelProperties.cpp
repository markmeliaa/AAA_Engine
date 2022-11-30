#include "Application.h"
#include "PanelProperties.h"
#include "ModuleWindow.h"
#include "ModuleEditor.h"
#include "ModuleTexture.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"

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
		ImGui::Text("Position	");
		ImGui::SameLine();
		static float pos[3] = { App->camera->GetModelTrans().x, App->camera->GetModelTrans().y ,App->camera->GetModelTrans().z };
		if (ImGui::DragFloat3(" ", pos, 0.005f))
		{
			App->camera->SetModelTrans(float3(pos[0], pos[1], pos[2]));
		}

		ImGui::Text("Rotation	");
		ImGui::SameLine();
		static float rot[3] = { App->camera->GetModelRotX(), App->camera->GetModelRotY(), App->camera->GetModelRotZ()};
		if (ImGui::DragFloat3("  ", rot, 0.005f))
		{
			App->camera->SetModelRot(rot[0], rot[1], rot[2]);
		}

		ImGui::Text("Scale	   ");
		ImGui::SameLine();
		static float scale[3] = { App->camera->GetModelScale().x, App->camera->GetModelScale().y, App->camera->GetModelScale().z };
		if (ImGui::DragFloat3("   ", scale, 0.005f))
		{
			App->camera->SetModelScale(float3(scale[0], scale[1], scale[2]));
		}

		ImGui::Separator();
	}

	if (ImGui::CollapsingHeader("Geometry"))
	{
		ImGui::TextUnformatted("AMOUNT OF VERTICES PER MESH:");
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
		ImGui::TextUnformatted("TOTAL AMOUNT OF VERTICES IN THE MODEL:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f),
			std::to_string(App->renderer->GetModel()->GetNumberVertices()).c_str());
		ImGui::Separator();

		ImGui::TextUnformatted("AMOUNT OF TRIANGLES PER MESH:");
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
		ImGui::TextUnformatted("TOTAL AMOUNT OF TRIANGLES IN THE MODEL:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f),
			std::to_string(App->renderer->GetModel()->GetNumberIndices() / 3).c_str());
		ImGui::Separator();
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