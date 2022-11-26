#pragma once
#pragma warning( disable : 4267 )
#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleDebugDraw.h"
#include "ModuleRenderExercise.h"
#include "ModuleEditor.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "ModuleTexture.h"

#include "GL/glew.h"
#include "DirectXTex.h"

ModuleRenderExercise::ModuleRenderExercise()
{
}

ModuleRenderExercise::~ModuleRenderExercise()
{
}

bool ModuleRenderExercise::Init()
{
	return true;
}

bool ModuleRenderExercise::Start()
{
	D_LOG("Creating and linking program");
	App->editor->log.emplace_back("Creating and linking program");
	program = CreateProgram(App->program->vertex_shader, App->program->fragment_shader);

	D_LOG("Creating triangle vertex buffer object");
	App->editor->log.emplace_back("Creating triangle vertex buffer object");

	CreateQuadBuffers();
	RenderQuad(program);

	return true;
}

update_status ModuleRenderExercise::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::Update()
{
	RenderQuad(program);
	
	App->draw->Draw(App->camera->GetViewMatrix(), App->camera->GetProjMatrix(), App->window->getCurrentWidth(), App->window->getCurrentHeight());

	return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp()
{
	D_LOG("Deleting buffer objects");
	App->editor->log.emplace_back("Deleting buffer objects");

	glDeleteProgram(program);
	DestroyVBO(vbo);
	DestroyEBO(ebo);
	DestroyVAO(vao);

	return true;
}

unsigned ModuleRenderExercise::CreateProgram(unsigned vtx_shader, unsigned frg_shader)
{
	unsigned program_id = glCreateProgram();
	glAttachShader(program_id, vtx_shader);
	glAttachShader(program_id, frg_shader);
	glLinkProgram(program_id);
	int res;
	glGetProgramiv(program_id, GL_LINK_STATUS, &res);
	if (res == GL_FALSE)
	{
		int len = 0;
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			int written = 0;
			char* info = (char*)malloc(len);
			glGetProgramInfoLog(program_id, len, &written, info);
			D_LOG("Program Log Info: %s", info);
			App->editor->log.emplace_back("Program Log Info:");
			App->editor->log.emplace_back(info);
			free(info);
		}
	}
	glDeleteShader(vtx_shader);
	glDeleteShader(frg_shader);
	return program_id;
}

// This function must be called one time at creation of vertex buffer
void ModuleRenderExercise::CreateQuadBuffers()
{
	float vertices[] = {
		// positions				// texture coords
		 2.0f,  2.0f, 0.0f,			1.0f,  1.0f,		// top right
		 2.0f, -2.0f, 0.0f,			1.0f,  0.0f,		// bottom right
		-2.0f, -2.0f, 0.0f,			0.0f,  0.0f,		// bottom left
		-2.0f,  2.0f, 0.0f,			0.0f,  1.0f			// top left 
	};

	unsigned int indices[] = {
		3, 1, 0,   // first triangle
		3, 2, 1    // second triangle
	};

	// Vertex Array Object
	glGenVertexArrays(1, &vao);
	glBindBuffer(GL_ARRAY_BUFFER, vao);

	// Vertex Buffer Object
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Element Buffer Object
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

// This function must be called each frame for drawing the triangle
void ModuleRenderExercise::RenderQuad(unsigned program)
{
	glUseProgram(program);

	// Retrieve model, view and projection matrix
	float4x4 model = float4x4::identity;
	//float4x4 model = float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f), float4x4::RotateZ(pi / 4.0f), float3(2.0f, 1.0f, 0.0f));
	float4x4 view = App->camera->GetViewMatrix();
	float4x4 proj = App->camera->GetProjMatrix();

	glUniformMatrix4fv(1, 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(2, 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(3, 1, GL_TRUE, &proj[0][0]);

	// Bind buffers and vertex attributes
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Activate the texture
	glActiveTexture(GL_TEXTURE5);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

// This function must be called one time at destruction of vertex buffer object
void ModuleRenderExercise::DestroyVBO(unsigned vbo)
{
	glDeleteBuffers(1, &vbo);
}

// This function must be called one time at destruction of element buffer object
void ModuleRenderExercise::DestroyEBO(unsigned ebo)
{
	glDeleteBuffers(1, &ebo);
}

// This function must be called one time at destruction of vertex array object
void ModuleRenderExercise::DestroyVAO(unsigned vao)
{
	glDeleteBuffers(1, &vao);
}