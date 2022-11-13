#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleDebugDraw.h"
#include "ModuleRenderExercise.h"
#include "ModuleEditor.h"

#include "lib/glew-2.1.0/include/GL/glew.h"
#include "lib/DirectXTex/DirectXTex/DirectXTex.h"

ModuleRenderExercise::ModuleRenderExercise()
{
	aspect = BASE_SCREEN_WIDTH / BASE_SCREEN_HEIGHT;
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
	vbo = CreateQuadVBO();

	SetUpFrustum();

	proj = frustum.ProjectionMatrix();
	model = float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f), float4x4::RotateZ(pi / 4.0f), float3(2.0f, 1.0f, 0.0f));
	view = frustum.ViewMatrix();

	return true;
}

update_status ModuleRenderExercise::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::Update()
{
	RenderTriangleVBO(vbo, program);
	
	App->draw->Draw(view, proj, BASE_SCREEN_WIDTH, BASE_SCREEN_HEIGHT);

	return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp()
{
	D_LOG("Deleting vertex buffer object");
	App->editor->log.emplace_back("Deleting vertex buffer object");

	DestroyVBO(vbo);

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
unsigned ModuleRenderExercise::CreateQuadVBO()
{
	float vtx_data[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		 0.0f, -2.0f, 0.0f,
		 1.0f, -1.0f, 0.0f
	};

	unsigned vbo;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);

	return vbo;
}

// This function must be called each frame for drawing the triangle
void ModuleRenderExercise::RenderTriangleVBO(unsigned vbo, unsigned program)
{
	// TODO: retrieve model view and projection
	glUseProgram(program);
	glUniformMatrix4fv(1, 1, GL_TRUE, &proj[0][0]);
	glUniformMatrix4fv(2, 1, GL_TRUE, &view[0][0]);	
	glUniformMatrix4fv(3, 1, GL_TRUE, &model[0][0]);

	// TODO: bind buffer and vertex attributes
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);

	// size = 3 float per vertex
	// stride = 0 is equivalent to stride = sizeof(float)*3
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glUseProgram(program);
	// 1 triangle to draw = 3 vertices
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

// This function must be called one time at destruction of vertex buffer
void ModuleRenderExercise::DestroyVBO(unsigned vbo)
{
	glDeleteBuffers(1, &vbo);
}

void ModuleRenderExercise::SetUpFrustum()
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.1f, 100.0f);
	frustum.SetHorizontalFovAndAspectRatio(DEGTORAD * 90.0f, 1.3f);

	frustum.SetPos(float3(0.0f, 4.0f, 8.0f));
	frustum.SetFront(-float3::unitZ);
	frustum.SetUp(float3::unitY);
}