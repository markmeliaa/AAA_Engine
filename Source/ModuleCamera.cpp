#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleEditor.h"
#include "ModuleInput.h"

#include <GL/glew.h>
#include "lib/MathGeoLib/Geometry/Frustum.h"
#include "SDL_scancode.h"

ModuleCamera::ModuleCamera()
{
}

ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
	D_LOG("Create the camera");
	App->editor->log.emplace_back("Creating the camera");

	SetUpFrustum();

	proj = frustum.ProjectionMatrix();
	model = float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f), float4x4::RotateZ(pi / 4.0f), float3(2.0f, 1.0f, 0.0f));
	view = frustum.ViewMatrix();

	float4x4 projectionGL = frustum.ProjectionMatrix().Transposed(); // < --Important to transpose!
	//Send the frustum projection matrix to OpenGLdirect mode would be:
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(*projectionGL.v);

	return true;
}

bool ModuleCamera::Start()
{
	return true;
}

update_status ModuleCamera::PreUpdate()
{
	//Send the frustum view matrix to OpenGL direct mode would be:
	float4x4 viewGL = float4x4(frustum.ViewMatrix()).Transposed();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(*viewGL.v);

	return UPDATE_CONTINUE;
}

update_status ModuleCamera::Update()
{
	float delta_time = App->GetDeltaTime();

	if (App->input->GetRightInput())
	{
		//App->editor->log.emplace_back("Right arrow pressed!!");
		Translate(frustum.WorldRight().Normalized() * move_speed * delta_time);
	}

	if (App->input->GetLeftInput())
	{
		//App->editor->log.emplace_back("Left arrow pressed!!");
		Translate(frustum.WorldRight().Normalized() * -move_speed * delta_time);
	}

	if (App->input->GetUpInput())
	{
		//App->editor->log.emplace_back("Left arrow pressed!!");
		Translate(frustum.Up().Normalized() * move_speed * delta_time);
	}

	if (App->input->GetDownInput())
	{
		//App->editor->log.emplace_back("Left arrow pressed!!");
		Translate(frustum.Up().Normalized() * -move_speed * delta_time);
	}

	return UPDATE_CONTINUE;
}

update_status ModuleCamera::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
	return true;
}

void ModuleCamera::SetUpFrustum()
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.1f, 2000.0f);
	frustum.SetHorizontalFovAndAspectRatio(DEGTORAD * 90.0f, 1.3f);

	SetPos(0.0f, 4.0f, 8.0f);
	frustum.SetFront(-float3::unitZ);
	frustum.SetUp(float3::unitY);
}

float4x4 ModuleCamera::GetModelMatrix() const
{
	return model;
}

float4x4 ModuleCamera::GetViewMatrix() const
{
	return frustum.ViewMatrix();
}

float4x4 ModuleCamera::GetProjMatrix() const
{
	return frustum.ProjectionMatrix();
}

void ModuleCamera::SetPos(const float3& newpos)
{
	frustum.SetPos(newpos);
}

void ModuleCamera::SetPos(const float& x, const float& y, const float& z)
{
	frustum.SetPos(float3(x, y, z));
}

void ModuleCamera::Translate(const float3& translation)
{
	frustum.SetPos(frustum.Pos() + translation);
}

void ModuleCamera::Rotate(const float3x3& rotationDeltaMatrix)
{
	vec oldFront = frustum.Front().Normalized();
	frustum.SetFront(rotationDeltaMatrix.MulDir(oldFront));
	vec oldUp = frustum.Up().Normalized();
	frustum.SetUp(rotationDeltaMatrix.MulDir(oldUp));
}