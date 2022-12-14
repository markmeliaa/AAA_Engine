#include "Application.h"
#include "Model.h"
#include "Mesh.h"
#include "ModuleTexture.h"
#include "ModuleEditor.h"

#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <Math/float3.h>
#include <string>
#include <assert.h>

void AssimpCallback(const char* info, char* data) {
	D_LOG("[ASSIMP] %s", info);
}

Model::Model(const char* file_name)
{
	struct aiLogStream assimp_stream;
	assimp_stream.callback = AssimpCallback;
	aiAttachLogStream(&assimp_stream);

	Load(file_name);
}

Model::~Model()
{
	D_LOG("Delete the textures, the meshes and the buffers at the end");
	App->editor->log.emplace_back("Delete the textures, the meshes and the buffers at the end");

	for (int i = 0; i < materials.size(); ++i) 
	{
		glDeleteTextures(1, &materials[i]);
	}
	materials.clear();

	for (int i = 0; i < meshes.size(); ++i) 
	{
		delete meshes[i];
	}
	meshes.clear();
}

void Model::Draw()
{
	for (int i = 0; i < meshes.size(); ++i)
	{
		meshes[i]->Draw(materials);
	}
}

void Model::Load(const char* file_name)
{
	assert(file_name != nullptr);

	const aiScene* scene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene)
	{
		D_LOG("Load the materials and the meshes of the model");
		App->editor->log.emplace_back("Load the materials and the meshes of the model");
		LoadMaterials(scene->mMaterials, scene->mNumMaterials, file_name);
		LoadMeshes(scene->mMeshes, scene->mNumMeshes);
		CreateModelSphere(scene->mMeshes, scene->mNumMeshes);

		D_LOG("Once everything is loaded, draw the model and adjust the view to its size");
		App->editor->log.emplace_back("Once loaded, draw the model and adjust the view to its size");
	}
	else
	{
		D_LOG("Error loading %s: %s", file_name, aiGetErrorString());
		App->editor->log.emplace_back("Error loading");
		App->editor->log.emplace_back(file_name);
	}
}

void Model::LoadMaterials(aiMaterial** materials, unsigned int numMaterials, const char* file_name)
{
	assert(materials != nullptr && file_name != nullptr);

	aiString file;
	this->materials.reserve(numMaterials);

	D_LOG("TRY TO LOAD TEXTURE");
	App->editor->log.emplace_back("-- TRY TO LOAD TEXTURE --");
	for (unsigned i = 0; i < numMaterials; ++i)
	{
		if (materials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS)
		{
			D_LOG("* First, try to load the texture on the path described by the FBX");
			App->editor->log.emplace_back("* First, try to load the texture on the path described by the FBX");

			std::string string_file = file.C_Str();
			int last_bar = 0;

			for (int i = 0; i < string_file.length(); ++i)
			{
				if (string_file[i] == 47 || string_file[i] == 92)
				{
					last_bar = i;
					last_bar++;
				}
			}

			std::string short_file = string_file.substr(last_bar, string_file.length() - last_bar);
			texture_name = short_file;

			current_texture = App->texture->LoadTexture(short_file.c_str());

			if (current_texture == 0)
			{
				D_LOG("* If that fails, load the texture on the same folder as the FBX");
				App->editor->log.emplace_back("* If that fails, load the texture on the same folder as the FBX");

				std::string model_file_dir = file_name;
				size_t last_slash = model_file_dir.find_last_of('\\');
				if (last_slash == std::string::npos)
				{
					last_slash = model_file_dir.find_last_of('/');
				}
				std::string model_folder_dir = model_file_dir.substr(0, last_slash + 1);
				std::string model_folder_material_file_dir = model_folder_dir + short_file;

				current_texture = App->texture->LoadTexture(model_folder_material_file_dir.c_str());
			}

			// Try to load relative to the textures folder
			if (current_texture == 0)
			{
				D_LOG("* Last, if both fail, load the texture on the Textures/ folder");
				App->editor->log.emplace_back("* Last, if both fail, load the texture on the Textures/ folder");

				std::string textures_folder_dir = "Textures/";
				std::string textures_folder_material_file_dir = textures_folder_dir + short_file;
				current_texture = App->texture->LoadTexture(textures_folder_material_file_dir.c_str());
			}

			if (current_texture == 0)
			{
				D_LOG("Unable to correctly load the given texture");
				App->editor->log.emplace_back("-- !!! Unable to correctly load the given texture !!! --");
			}

			else
			{
				D_LOG("TEXTURE LOADED CORRECTLY");
				App->editor->log.emplace_back("-- TEXTURE LOADED CORRECTLY --");
			}

			this->materials.emplace_back(current_texture);
		}
	}
}

void Model::LoadMeshes(aiMesh** meshes, unsigned int numMeshes)
{
	assert(meshes != nullptr);

	this->meshes.reserve(numMeshes);

	D_LOG("For each mesh, create and load its VBO, EBO and VAO");
	App->editor->log.emplace_back("For each mesh, create and load its VBO, EBO and VAO");
	for (unsigned int i = 0; i < numMeshes; ++i)
	{
		this->meshes.emplace_back(new Mesh(meshes[i]));
	}
}

void Model::CreateModelSphere(aiMesh** meshes, unsigned int numMeshes)
{
	assert(meshes != nullptr);

	for (int i = 0; i < this->meshes.size(); ++i)
	{
		total_num_vertices += this->meshes[i]->GetNumVertices();
		total_num_indices += this->meshes[i]->GetNumIndices();
	}

	std::vector<float3> sphere_vertices;
	sphere_vertices.reserve(total_num_vertices);
	for (unsigned int i = 0; i < numMeshes; ++i)
	{
		const aiMesh* mesh = meshes[i];

		for (unsigned int j = 0; j < mesh->mNumVertices; j++)
		{
			const aiVector3D& vertex = mesh->mVertices[j];
			sphere_vertices.push_back(vec(vertex.x, vertex.y, vertex.z));
		}
	}

	base_model_bounds = Sphere::OptimalEnclosingSphere(sphere_vertices.data(), (int)sphere_vertices.size());
	current_model_bounds = base_model_bounds;
}