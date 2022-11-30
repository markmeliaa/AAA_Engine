#pragma once
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleEditor.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_scancode.h>
#include <Geometry/Sphere.h>

ModuleCamera::ModuleCamera()
{
	frustum = new Frustum();
	//model = float4x4::identity;
	model_trans = float3(0.0f, 0.0f, 0.0f);
	model_rot = float4x4::RotateX(rotX) * float4x4::RotateY(rotY) * float4x4::RotateY(rotZ);
	model_scale = float3(1.0f, 1.0f, 1.0f);
}

ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
	D_LOG("Create the camera");
	App->editor->log.emplace_back("Creating the camera");

	aspectRatio = (float)App->window->getCurrentWidth() / (float)App->window->getCurrentHeight();
	SetUpFrustum();

	proj = frustum->ProjectionMatrix();
	view = frustum->ViewMatrix();
	model = float4x4::FromTRS(model_trans, model_rot, model_scale);

	return true;
}

bool ModuleCamera::Start()
{
	SDL_WarpMouseInWindow(App->window->window, App->window->getCurrentWidth() / 2, App->window->getCurrentHeight() / 2);
	return true;
}

update_status ModuleCamera::Update()
{
	aspectRatio = (float)App->window->getCurrentWidth() / (float)App->window->getCurrentHeight();
	frustum->SetHorizontalFovAndAspectRatio(frustum->HorizontalFov(), aspectRatio);

	model = float4x4::FromTRS(model_trans, model_rot, model_scale);

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

	// Focus model with F
	if (App->input->GetKey(SDL_SCANCODE_F))
	{
		Focus(App->renderer->GetModel());
	}

	// Move camera around while right clicking (translations) with WASD + QE
	if (App->input->GetMouseButton(3))
	{
		if (App->input->GetKey(SDL_SCANCODE_A))
		{
			Translate(frustum->WorldRight().Normalized() * -move_speed_inc * delta_time);
		}

		if (App->input->GetKey(SDL_SCANCODE_D))
		{
			Translate(frustum->WorldRight().Normalized() * move_speed_inc * delta_time);
		}

		if (App->input->GetKey(SDL_SCANCODE_Q))
		{
			Translate(frustum->Up().Normalized() * move_speed_inc * delta_time);
		}

		if (App->input->GetKey(SDL_SCANCODE_E))
		{
			Translate(frustum->Up().Normalized() * -move_speed_inc * delta_time);
		}

		if (App->input->GetKey(SDL_SCANCODE_W))
		{
			Translate(frustum->Front().Normalized() * move_speed_inc * delta_time);
		}

		if (App->input->GetKey(SDL_SCANCODE_S))
		{
			Translate(frustum->Front().Normalized() * -move_speed_inc * delta_time);
		}
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
		if (frustum->Front().y < 0.75f)
			Rotate(float3x3::RotateAxisAngle(frustum->WorldRight().Normalized(), rotate_speed_inc * DEGTORAD * delta_time));
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN))
	{
		if (frustum->Front().y > -0.75f)
			Rotate(float3x3::RotateAxisAngle(frustum->WorldRight().Normalized(), -rotate_speed_inc * DEGTORAD * delta_time));
	}

	// Move camera around (rotations) with Mouse Control
	if (App->input->GetMouseButton(1))
	{
		Rotate(float3x3::RotateY(mouse_input.x * move_speed_inc * DEGTORAD * delta_time));		
		Rotate(float3x3::RotateAxisAngle(frustum->WorldRight().Normalized(), mouse_input.y * move_speed_inc * DEGTORAD * delta_time));
	}

	// Move camera onwards and backwards with Mouse Wheel
	if (App->input->GetMouseWheel())
	{
		Translate(frustum->Front().Normalized() * move_speed_inc * delta_time * App->input->GetMouseWheelInput().y);
		App->input->SetMouseWheel(false);
	}

	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
	delete frustum;
	return true;
}

void ModuleCamera::SetUpFrustum()
{
	frustum->SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum->SetViewPlaneDistances(0.1f, 1000.0f);
	frustum->SetHorizontalFovAndAspectRatio(DEGTORAD * 90.0f, aspectRatio);

	SetPos(0.0f, 3.0f, 10.0f);
	frustum->SetFront(-float3::unitZ);
	frustum->SetUp(float3::unitY);
}

float4x4 ModuleCamera::GetViewMatrix() const
{
	return frustum->ViewMatrix();
}

float4x4 ModuleCamera::GetProjMatrix() const
{
	return frustum->ProjectionMatrix();
}

float4x4 ModuleCamera::GetModelMatrix() const
{
	return model;
}

void ModuleCamera::SetPos(const float3& newpos)
{
	frustum->SetPos(newpos);
}

void ModuleCamera::SetPos(const float& x, const float& y, const float& z)
{
	frustum->SetPos(float3(x, y, z));
}

void ModuleCamera::Translate(const float3& translation)
{
	SetPos(frustum->Pos() + translation);
}

void ModuleCamera::Translate(const float& x, const float& y, const float& z)
{
	SetPos(frustum->Pos() + float3(x, y, z));
}

void ModuleCamera::Rotate(const float3x3& rotationDeltaMatrix)
{
	vec oldFront = frustum->Front().Normalized();
	frustum->SetFront(rotationDeltaMatrix.MulDir(oldFront));
	vec oldUp = frustum->Up().Normalized();
	frustum->SetUp(rotationDeltaMatrix.MulDir(oldUp));
}

void ModuleCamera::Focus(Model* model)
{
	// Set the camera in the center of the model
	SetPos(model_trans.x + model->GetModelBounds().Centroid().x * Abs(model_scale.x),
		   model_trans.y + model->GetModelBounds().Centroid().y * model_scale.y,
		   model_trans.z + model->GetModelBounds().Centroid().z * Abs(model_scale.z));

	// Move the camera away from the center of the model a certain distance
	float away = model->GetModelBounds().r * Max(Abs(model_scale.x), Abs(model_scale.y), Abs(model_scale.z));
	Translate(-away * 1.4f, 0.0f, away * 1.4f);

	frustum->SetFront((float3::unitX + -float3::unitZ).Normalized());
	frustum->SetUp(float3::unitY);
}

float3 ModuleCamera::GetModelTrans() const
{
	return model_trans;
}

float ModuleCamera::GetModelRotX() const
{
	return rotX;
}

float ModuleCamera::GetModelRotY() const
{
	return rotY;
}

float ModuleCamera::GetModelRotZ() const
{
	return rotZ;
}

float3 ModuleCamera::GetModelScale() const
{
	return model_scale;
}

void ModuleCamera::SetModelTrans(float3 t)
{
	model_trans = t;
}

void ModuleCamera::SetModelRot(float x, float y, float z)
{
	model_rot = float4x4::RotateX(x) * float4x4::RotateY(y) * float4x4::RotateZ(z);
}

void ModuleCamera::SetModelScale(float3 s)
{
	model_scale = s;
}