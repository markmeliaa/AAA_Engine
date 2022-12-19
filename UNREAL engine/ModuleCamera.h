#ifndef __ModuleCamera_H__
#define __ModuleCamera_H__

#include "Module.h"

#include <Geometry/Frustum.h>
#include <Math/float3x3.h>

class Model;

class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera() override;

	bool Init() override;
	bool Start() override;
	update_status Update() override;
	bool CleanUp() override;

	float4x4 GetViewMatrix() const { return frustum->ViewMatrix(); }
	float4x4 GetProjMatrix() const { return frustum->ProjectionMatrix(); }
	float4x4 GetModelMatrix() const { return model; }

	const float3& GetModelTrans() const { return model_trans; }
	void SetModelTrans(const float3& t) { model_trans = t; }

	float GetModelRotX() const { return rotX; }
	float GetModelRotY() const { return rotY; }
	float GetModelRotZ() const { return rotZ; }
	void SetModelRotX(float x) { rotX = x; }
	void SetModelRotY(float y) { rotY = y; }
	void SetModelRotZ(float z) { rotZ = z; }
	void SetModelRot(float x, float y, float z);

	const float3& GetModelScale() const { return model_scale; }
	void SetModelScale(const float3& s) { model_scale = s; }

	void ResetModelMat();

	float GetNearPlane() const { return frustum->NearPlaneDistance(); }
	void SetNearPlane(float np) { frustum->SetViewPlaneDistances(np, frustum->FarPlaneDistance()); }
	float GetFarPlane() const { return frustum->FarPlaneDistance(); }
	void SetFarPlane(float fp) { frustum->SetViewPlaneDistances(frustum->NearPlaneDistance(), fp); }

	float GetMoveSpeed() const { return move_speed; }
	void SetMoveSpeed(float m) { move_speed = m; }
	float GetRotSpeed() const { return rotate_speed; }
	void SetRotSpeed(float r) { rotate_speed = r; }
	float GetZoomSpeed() const { return zoom_speed; }
	void SetZoomSpeed(float z) { zoom_speed = z; }

	void Focus(const Model* model);

private:
	void InitFrustum();

	void SetPos(const float3& newpos) { frustum->SetPos(newpos); }
	void SetPos(const float& x, const float& y, const float& z) { frustum->SetPos(float3(x, y, z)); }

	void Translate(const float3& translation) { SetPos(frustum->Pos() + translation); }
	void Translate(const float& x, const float& y, const float& z) { SetPos(frustum->Pos() + float3(x, y, z)); }

	void Rotate(const float3x3& rotationDeltaMatrix);
	void Orbit(const Model* model, const float2& mouse_input, const float& rotate_speed);
	void LookAt(const float3& objective);

	float aspectRatio = 0.0f;
	Frustum* frustum;

	float4x4 model;
	float3 model_trans, model_scale;
	float rotX, rotY, rotZ = 0.0f;
	float4x4 model_rot;

	float move_speed = 3.0f;
	float rotate_speed = 15.0f;
	float zoom_speed = 5.0f;
};

#endif // __ModuleCamera_H__