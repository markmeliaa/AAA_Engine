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
#include <Math/Quat.h>

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
	App->editor->log.emplace_back("Create the camera");

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
	float zoom_speed_inc = zoom_speed;

	float2 mouse_input = App->input->GetMouseInput();

	// Increase speed with LShift
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT))
	{
		move_speed_inc *= 2;
		rotate_speed_inc *= 2;
		zoom_speed_inc *= 2;
	}

	// Focus model with F
	if (App->input->GetKey(SDL_SCANCODE_F))
	{
		Focus(App->renderer->GetModel());
	}

	// Move camera around while right clicking (translations) and WASD + QE pressed
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
			Translate(frustum->Front().Normalized() * zoom_speed_inc * delta_time);
		}

		if (App->input->GetKey(SDL_SCANCODE_S))
		{
			Translate(frustum->Front().Normalized() * -zoom_speed_inc * delta_time);
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


	if (App->input->GetKey(SDL_SCANCODE_LALT) && (App->input->GetMouseButton(1)))
	{
		Orbit(App->renderer->GetModel(), mouse_input, rotate_speed_inc);
	}

	// Move camera around (rotations) with Mouse Control
	if (App->input->GetMouseButton(1) && !App->input->GetKey(SDL_SCANCODE_LALT))
	{
		Rotate(float3x3::RotateY((mouse_input.x * 0.1f) * rotate_speed_inc * DEGTORAD * delta_time));		
		Rotate(float3x3::RotateAxisAngle(frustum->WorldRight().Normalized(), (mouse_input.y * 0.1f) * rotate_speed_inc * DEGTORAD * delta_time));
	}

	// Move camera onwards and backwards with Mouse Wheel
	if (App->input->GetMouseWheel())
	{
		Translate(frustum->Front().Normalized() * zoom_speed_inc * delta_time * App->input->GetMouseWheelInput().y);
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

void ModuleCamera::Focus(const Model* model)
{
	// Set the camera in the center of the model
	SetPos(model_trans.x + model->GetCurrentModelBounds().Centroid().x * Abs(model_scale.x),
		   model_trans.y + model->GetCurrentModelBounds().Centroid().y * model_scale.y,
		   model_trans.z + model->GetCurrentModelBounds().Centroid().z * Abs(model_scale.z));

	// Move the camera away from the center of the model a certain distance
	float away = model->GetCurrentModelBounds().r * Max(Abs(model_scale.x), Abs(model_scale.y), Abs(model_scale.z));
	Translate(-away * 1.4f, 0.0f, away * 1.4f);

	frustum->SetFront((float3::unitX + -float3::unitZ).Normalized());
	frustum->SetUp(float3::unitY);
}

void ModuleCamera::Orbit(const Model* model, const float2& mouse_input, const float& rotate_speed)
{
	float3 center_sphere_model = App->renderer->GetModel()->GetCurrentModelBounds().Centroid();
	float3 distance_from_model = frustum->Pos() - center_sphere_model;

	Quat rotX = Quat(frustum->Up(), -mouse_input.x * rotate_speed * App->GetDeltaTime() * DEGTORAD);
	Quat rotY = Quat(frustum->WorldRight(), -mouse_input.y * rotate_speed * App->GetDeltaTime() * DEGTORAD);

	// Set up vertical rotation limits
	float cos_angle = Dot(frustum->Front(), float3::unitY);
	if (cos_angle > 0.75f) 
	{
		rotY = Quat(frustum->WorldRight(), -0.5 * DEGTORAD);
	}

	else if (cos_angle < -0.75f) 
	{
		rotY = Quat(frustum->WorldRight(), 0.5 * DEGTORAD);
	}

	distance_from_model = rotX.Transform(distance_from_model);
	distance_from_model = rotY.Transform(distance_from_model);

	frustum->SetPos(distance_from_model + center_sphere_model);
	LookAt(center_sphere_model);
}

void ModuleCamera::LookAt(float3 objective)
{
	float3 direction = objective - frustum->Pos();
	float3x3 looking_direction = float3x3::LookAt(frustum->Front(), direction.Normalized(), frustum->Up(), float3::unitY);

	Rotate(looking_direction);
}

float3 ModuleCamera::GetModelTrans() const
{
	return model_trans;
}

void ModuleCamera::SetModelTrans(const float3& t)
{
	model_trans = t;
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

void ModuleCamera::SetModelRotX(const float& x)
{
	rotX = x;
}

void ModuleCamera::SetModelRotY(const float& y)
{
	rotY = y;
}

void ModuleCamera::SetModelRotZ(const float& z)
{
	rotZ = z;
}

void ModuleCamera::SetModelRot(const float& x, const float& y, const float& z)
{
	model_rot = float4x4::RotateX(x) * float4x4::RotateY(y) * float4x4::RotateZ(z);
}

float3 ModuleCamera::GetModelScale() const
{
	return model_scale;
}

void ModuleCamera::SetModelScale(const float3& s)
{
	model_scale = s;
}

void ModuleCamera::ResetModelMat()
{
	SetModelTrans(float3(0.0f, 0.0f, 0.0f));
	SetModelRotX(0.0f);
	SetModelRotY(0.0f);
	SetModelRotZ(0.0f);
	SetModelRot(0.0f, 0.0f, 0.0f);
	SetModelScale(float3(1.0f, 1.0f, 1.0f));
}

float ModuleCamera::GetNearPlane() const
{
	return frustum->NearPlaneDistance();
}

void ModuleCamera::SetNearPlane(const float& np)
{
	frustum->SetViewPlaneDistances(np, frustum->FarPlaneDistance());
}

float ModuleCamera::GetFarPlane() const
{
	return frustum->FarPlaneDistance();
}

void ModuleCamera::SetFarPlane(const float& fp)
{
	frustum->SetViewPlaneDistances(frustum->NearPlaneDistance(), fp);
}

float ModuleCamera::GetMoveSpeed() const
{
	return move_speed;
}

void ModuleCamera::SetMoveSpeed(const float& m)
{
	move_speed = m;
}

float ModuleCamera::GetRotSpeed() const
{
	return rotate_speed;
}

void ModuleCamera::SetRotSpeed(const float& r)
{
	rotate_speed = r;
}

float ModuleCamera::GetZoomSpeed() const
{
	return zoom_speed;
}

void ModuleCamera::SetZoomSpeed(const float& z)
{
	zoom_speed = z;
}