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
	float4x4 GetModelMatrix() const;

	float3 GetModelTrans() const;
	float GetModelRotX() const;
	float GetModelRotY() const;
	float GetModelRotZ() const;
	float3 GetModelScale() const;

	void SetModelTrans(float3 t);
	void SetModelRot(float x, float y, float z);
	void SetModelScale(float3 s);

private:
	void SetUpFrustum();

	void SetPos(const float3& newpos);
	void SetPos(const float& x, const float& y, const float& z);

	void Translate(const float3& translation);
	void Rotate(const float3x3& rotationDeltaMatrix);

	float aspectRatio = 0.0f;
	Frustum* frustum;
	float4x4 model, view, proj;

	float3 model_trans, model_scale;
	float rotX, rotY, rotZ = 0.0f;
	float4x4 model_rot;

	float move_speed = 4.0f;
	float rotate_speed = 25.0f;
};

#endif // __ModuleCamera_H__