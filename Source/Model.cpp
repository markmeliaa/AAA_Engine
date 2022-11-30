#pragma once
#include "Application.h"
#include "Model.h"
#include "ModuleTexture.h"
#include "ModuleEditor.h"

#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <Math/float3.h>

Model::Model(const char* file_name)
{
	Load(file_name);
}

Model::~Model()
{
	D_LOG("Delete the textures, the meshes and the buffers at the end");
	App->editor->log.emplace_back("Delete the textures, the meshes and the buffers at the end");

	for (int i = 0; i < materials.size(); i++) 
	{
		glDeleteTextures(1, &materials[i]);
	}

	for (int i = 0; i < meshes.size(); i++) 
	{
		delete meshes[i];
	}
}

void Model::Draw()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->Draw(materials);
	}
}

void Model::Load(const char* file_name)
{
	const aiScene* scene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene)
	{
		D_LOG("Load the materials and the meshes of the model");
		App->editor->log.emplace_back("Load the materials and the meshes of the model");
		LoadMaterials(scene->mMaterials, scene->mNumMaterials);
		LoadMeshes(scene->mMeshes, scene->mNumMeshes);
		CreateModelSphere(scene->mMeshes, scene->mNumMeshes);

		D_LOG("Once loaded, draw the model");
		App->editor->log.emplace_back("Once loaded, draw the model");
	}
	else
	{
		D_LOG("Error loading %s: %s", file_name, aiGetErrorString());
		App->editor->log.emplace_back("Error loading");
		App->editor->log.emplace_back(file_name);
	}
}

void Model::LoadMaterials(aiMaterial** materials, const unsigned int& numMaterials)
{
	aiString file;
	this->materials.reserve(numMaterials);

	D_LOG("For each material, load its texture");
	App->editor->log.emplace_back("For each material, load its texture");
	for (unsigned i = 0; i < numMaterials; ++i)
	{
		if (materials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS)
		{
			this->materials.emplace_back(App->texture->LoadTexture(file.data));
		}
	}
}

void Model::LoadMeshes(aiMesh** meshes, const unsigned int& numMeshes)
{
	this->meshes.reserve(numMeshes);

	D_LOG("For each mesh, create and load its VBO, EBO and VAO");
	App->editor->log.emplace_back("For each mesh, create and load its VBO, EBO and VAO");
	for (unsigned int i = 0; i < numMeshes; ++i)
	{
		this->meshes.emplace_back(new Mesh(meshes[i]));
	}
}

void Model::CreateModelSphere(aiMesh** meshes, const unsigned int& numMeshes)
{
	for (int i = 0; i < this->meshes.size(); i++)
	{
		total_num_vertices += this->meshes[i]->GetNumVertices();
		total_num_indices += this->meshes[i]->GetNumIndices();
	}

	std::vector<float3> sphere_vertices;
	sphere_vertices.reserve(total_num_vertices);
	for (unsigned int i = 0; i < numMeshes; i++)
	{
		const aiMesh* mesh = meshes[i];

		for (unsigned int j = 0; j < mesh->mNumVertices; j++)
		{
			const aiVector3D& vertex = mesh->mVertices[j];
			sphere_vertices.push_back(vec(vertex.x, vertex.y, vertex.z));
		}
	}

	model_bounds = Sphere::FastEnclosingSphere(sphere_vertices.data(), (int)sphere_vertices.size());
}

std::vector<Mesh*> Model::GetMeshes() const
{
	return meshes;
}

int Model::GetNumberVertices() const
{
	return total_num_vertices;
}

int Model::GetNumberIndices() const
{
	return total_num_indices;
}

Sphere Model::GetModelBounds() const
{
	return model_bounds;
}