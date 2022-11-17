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

	return true;
}

bool ModuleCamera::Start()
{
	return true;
}

update_status ModuleCamera::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleCamera::Update()
{
	float delta_time = App->GetDeltaTime();
	float move_speed_inc = move_speed;
	float rotate_speed_inc = rotate_speed;

	// Increase speed with LShift
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT))
	{
		move_speed_inc *= 2;
		rotate_speed_inc *= 2;
	}

	// Move camera around (translations) with WASD + QE
	if (App->input->GetKey(SDL_SCANCODE_A))
	{
		Translate(frustum.WorldRight().Normalized() * -move_speed_inc * delta_time);
	}

	if (App->input->GetKey(SDL_SCANCODE_D))
	{
		Translate(frustum.WorldRight().Normalized() * move_speed_inc * delta_time);
	}

	if (App->input->GetKey(SDL_SCANCODE_Q))
	{
		Translate(frustum.Up().Normalized() * move_speed_inc * delta_time);
	}

	if (App->input->GetKey(SDL_SCANCODE_E))
	{
		Translate(frustum.Up().Normalized() * -move_speed_inc * delta_time);
	}

	if (App->input->GetKey(SDL_SCANCODE_W))
	{
		Translate(frustum.Front().Normalized() * move_speed_inc * delta_time);
	}

	if (App->input->GetKey(SDL_SCANCODE_S))
	{
		Translate(frustum.Front().Normalized() * -move_speed_inc * delta_time);
	}

	// Move camera around (rotations) with Arrow Keys
	if (App->input->GetKey(SDL_SCANCODE_LEFT))
	{
		Rotate(float3x3::RotateY(rotate_speed_inc * DEGTORAD * delta_time));
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT))
	{
		Rotate(float3x3::RotateY(-rotate_speed_inc * DEGTORAD * delta_time));
	}

	if (App->input->GetKey(SDL_SCANCODE_UP))
	{
		Rotate(float3x3::RotateX(rotate_speed_inc * DEGTORAD * delta_time));
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN))
	{
		Rotate(float3x3::RotateX(-rotate_speed_inc * DEGTORAD * delta_time));
	}

	// Move camera around (rotations) with Mouse Control

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
	SetPos(frustum.Pos() + translation);
}

void ModuleCamera::Rotate(const float3x3& rotationDeltaMatrix)
{
	vec oldFront = frustum.Front().Normalized();
	frustum.SetFront(rotationDeltaMatrix.MulDir(oldFront));
	vec oldUp = frustum.Up().Normalized();
	frustum.SetUp(rotationDeltaMatrix.MulDir(oldUp));
}