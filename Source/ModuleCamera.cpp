#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleEditor.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"

#include <GL/glew.h>
#include "lib/MathGeoLib/Geometry/Frustum.h"
#include "lib/SDL/include/SDL.h"
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
	SDL_WarpMouseInWindow(App->window->window, App->window->getCurrentWidth() / 2, App->window->getCurrentHeight() / 2);
	return true;
}

update_status ModuleCamera::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleCamera::Update()
{
	if (App->editor->IsAnyWindowsFocused())
		return UPDATE_CONTINUE;

	float delta_time = App->GetDeltaTime();

	float move_speed_inc = move_speed;
	float rotate_speed_inc = rotate_speed;

	float2 mouse_input = App->input->GetMouseInput();

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
		Rotate(float3x3::RotateAxisAngle(frustum.Up().Normalized(), rotate_speed_inc * DEGTORAD * delta_time));
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT))
	{
		Rotate(float3x3::RotateAxisAngle(frustum.Up().Normalized(), -rotate_speed_inc * DEGTORAD * delta_time));
	}

	if (App->input->GetKey(SDL_SCANCODE_UP))
	{
		Rotate(float3x3::RotateAxisAngle(frustum.WorldRight().Normalized(), rotate_speed_inc * DEGTORAD * delta_time));
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN))
	{
		Rotate(float3x3::RotateAxisAngle(frustum.WorldRight().Normalized(), -rotate_speed_inc * DEGTORAD * delta_time));
	}

	// Move camera around (rotations) with Mouse Control
	if (App->input->GetMouseButton(1))
	{
		Rotate(float3x3::RotateAxisAngle(frustum.Up().Normalized(), mouse_input.x * rotate_speed_inc * DEGTORAD * delta_time));
		Rotate(float3x3::RotateAxisAngle(frustum.WorldRight().Normalized(), mouse_input.y * rotate_speed_inc * DEGTORAD * delta_time));
	}

	// Reset position with F
	if (App->input->GetKey(SDL_SCANCODE_F))
	{
		SetPos(0.0f, 4.0f, 8.0f);
		frustum.SetFront(-float3::unitZ);
		frustum.SetUp(float3::unitY);
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
	frustum.SetViewPlaneDistances(0.1f, 1000.0f);
	frustum.SetHorizontalFovAndAspectRatio(DEGTORAD * 100.0f, 1.3f);

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

void ModuleCamera::SetFov(const float& fov)
{
	frustum.SetHorizontalFovAndAspectRatio(fov, frustum.AspectRatio());
}

float ModuleCamera::GetFov() const
{
	return frustum.HorizontalFov();
}

void ModuleCamera::SetAspectRatio(const float& aspect)
{
	frustum.SetHorizontalFovAndAspectRatio(frustum.HorizontalFov(), aspect);
}

float ModuleCamera::GetAspectRatio() const
{
	return frustum.AspectRatio();
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