#ifndef __ModuleCamera_H__
#define __ModuleCamera_H__

#include "Module.h"
#include "Globals.h"

#include <Geometry/Frustum.h>
#include <Math/float3x3.h>

class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool Init() override;
	bool Start() override;
	update_status Update() override;
	bool CleanUp() override;

	float4x4 GetViewMatrix() const;
	float4x4 GetProjMatrix() const;

private:
	void SetUpFrustum();

	void SetPos(const float3& newpos);
	void SetPos(const float& x, const float& y, const float& z);

	void Translate(const float3& translation);
	void Rotate(const float3x3& rotationDeltaMatrix);

	float aspectRatio = 0.0f;
	Frustum* frustum;
	float4x4 model, view, proj;

	float move_speed = 4.0f;
	float rotate_speed = 25.0f;
};

#endif // __ModuleCamera_H__