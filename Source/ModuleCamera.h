#pragma once
#include "Module.h"
#include "Globals.h"

#include "lib/MathGeoLib/Geometry/Frustum.h"
#include "lib/MathGeoLib/Math/float3x3.h"

class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool Init() override;
	bool Start() override;
	update_status PreUpdate() override;
	update_status Update() override;
	update_status PostUpdate() override;
	bool CleanUp() override;

	float4x4 GetViewMatrix() const;
	float4x4 GetProjMatrix() const;

	void SetHorFov(const float& fov);
	void SetVerFov(const float& fov);
	float GetHorFov() const;
	float GetVerFov() const;
	void SetAspectRatio(const float& aspect);
	float GetAspectRatio() const;

	void SetPos(const float3& newpos);
	void SetPos(const float& x, const float& y, const float& z);
	void Translate(const float3& translation);
	void Rotate(const float3x3& rotationDeltaMatrix);

private:
	void SetUpFrustum();
	Frustum* frustum;
	float4x4 model, view, proj;

	float aspectRatio = 0.0f;

	float move_speed = 4.0f;
	float rotate_speed = 25.0f;
};