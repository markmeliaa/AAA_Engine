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
#include <SDL.h>
#include <GL/glew.h>
#include <assimp/version.h>

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

	ImGui::SetNextWindowSize(ImVec2(400, (float)App->window->getCurrentHeight() / 2 - 10), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2((float)App->window->getCurrentWidth() - 400, (float)App->window->getCurrentHeight() / 2 + 18), ImGuiCond_Always);
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::Begin("Properties", &visible);

	if (ImGui::CollapsingHeader("Transformation"))
	{
		ImGui::Text("Position	");
		ImGui::SameLine();
		float pos[3] = { App->camera->GetModelTrans().x, App->camera->GetModelTrans().y ,App->camera->GetModelTrans().z };
		if (ImGui::DragFloat3(" ", pos, 0.005f))
		{
			App->camera->SetModelTrans(float3(pos[0], pos[1], pos[2]));
		}

		ImGui::Text("Rotation	");
		ImGui::SameLine();
		float rot[3] = { App->camera->GetModelRotX(), App->camera->GetModelRotY(), App->camera->GetModelRotZ()};
		if (ImGui::DragFloat3("  ", rot, 0.005f, ImGuiInputTextFlags_ReadOnly))
		{
			App->camera->SetModelRotX(rot[0]);
			App->camera->SetModelRotY(rot[1]);
			App->camera->SetModelRotZ(rot[2]);
			App->camera->SetModelRot(App->camera->GetModelRotX(), App->camera->GetModelRotY(), App->camera->GetModelRotZ());
		}

		ImGui::Text("Scale	   ");
		ImGui::SameLine();
		float scale[3] = { App->camera->GetModelScale().x, App->camera->GetModelScale().y, App->camera->GetModelScale().z };
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