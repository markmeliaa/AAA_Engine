#include "Application.h"
#include "Mesh.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"

#include <Math/float2.h>
#include <assert.h>
#include <assimp/mesh.h>

Mesh::Mesh(const aiMesh* mesh)
{
	LoadVBO(mesh);
	LoadEBO(mesh);
	CreateVAO();
	material_index = mesh->mMaterialIndex;
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}

void Mesh::Draw(const std::vector<GLuint>& model_textures)
{
	unsigned program = App->program->GetProgram();

	const float4x4 model = App->camera->GetModelMatrix();
	const float4x4& view = App->camera->GetViewMatrix();
	const float4x4& proj = App->camera->GetProjMatrix();

	glUseProgram(program);
	glUniformMatrix4fv(1, 1, GL_TRUE, (const float*)&model);
	glUniformMatrix4fv(2, 1, GL_TRUE, (const float*)&view);
	glUniformMatrix4fv(3, 1, GL_TRUE, (const float*)&proj);

	if (model_textures.size() != 0)
	{
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, model_textures[material_index]);
	}

	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, nullptr);
}

void Mesh::LoadVBO(const aiMesh* mesh)
{
	assert(mesh != nullptr);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	unsigned vertex_size = (sizeof(float) * 3 + sizeof(float) * 2);
	unsigned buffer_size = vertex_size * mesh->mNumVertices;
	glBufferData(GL_ARRAY_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);

	unsigned position_size = sizeof(float) * 3 * mesh->mNumVertices;
	glBufferSubData(GL_ARRAY_BUFFER, 0, position_size, mesh->mVertices);

	unsigned uv_offset = position_size;
	unsigned uv_size = sizeof(float) * 2 * mesh->mNumVertices;
	float2* uvs = (float2*)(glMapBufferRange(GL_ARRAY_BUFFER, uv_offset, uv_size, GL_MAP_WRITE_BIT));

	for (unsigned i = 0; i < mesh->mNumVertices; ++i)
	{
		uvs[i] = float2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);
	num_vertices = mesh->mNumVertices;
}

void Mesh::LoadEBO(const aiMesh* mesh)
{
	assert(mesh != nullptr);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	unsigned index_size = sizeof(unsigned) * mesh->mNumFaces * 3;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size, nullptr, GL_STATIC_DRAW);

	unsigned* indices = (unsigned*)(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
	for (unsigned i = 0; i < mesh->mNumFaces; ++i)
	{
		assert(mesh->mFaces[i].mNumIndices == 3); // note: assume triangles = 3 indices per face
		*(indices++) = mesh->mFaces[i].mIndices[0];
		*(indices++) = mesh->mFaces[i].mIndices[1];
		*(indices++) = mesh->mFaces[i].mIndices[2];
	}

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	num_indices = mesh->mNumFaces * 3;
}

void Mesh::CreateVAO()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * num_vertices));
}