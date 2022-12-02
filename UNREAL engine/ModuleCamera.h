#ifndef __ModuleCamera_H__
#define __ModuleCamera_H__

#include "Module.h"
#include "Globals.h"
#include "Model.h"

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

	inline float4x4 GetViewMatrix() const { return frustum->ViewMatrix(); }
	inline float4x4 GetProjMatrix() const { return frustum->ProjectionMatrix(); }
	inline float4x4 GetModelMatrix() const { return model; }

	inline float3 GetModelTrans() const { return model_trans; }
	inline void SetModelTrans(const float3& t) { model_trans = t; }

	inline float GetModelRotX() const { return rotX; }
	inline float GetModelRotY() const { return rotY; }
	inline float GetModelRotZ() const { return rotZ; }
	inline void SetModelRotX(const float& x) { rotX = x; }
	inline void SetModelRotY(const float& y) { rotY = y; }
	inline void SetModelRotZ(const float& z) { rotZ = z; }
	inline void SetModelRot(const float& x, const float& y, const float& z);

	inline float3 GetModelScale() const { return model_scale; }
	inline void SetModelScale(const float3& s) { model_scale = s; }

	void ResetModelMat();

	inline float GetNearPlane() const { return frustum->NearPlaneDistance(); }
	inline void SetNearPlane(const float& np) { frustum->SetViewPlaneDistances(np, frustum->FarPlaneDistance()); }
	inline float GetFarPlane() const { return frustum->FarPlaneDistance(); }
	inline void SetFarPlane(const float& fp) { frustum->SetViewPlaneDistances(frustum->NearPlaneDistance(), fp); }

	inline float GetMoveSpeed() const { return move_speed; }
	inline void SetMoveSpeed(const float& m) { move_speed = m; }
	inline float GetRotSpeed() const { return rotate_speed; }
	inline void SetRotSpeed(const float& r) { rotate_speed = r; }
	inline float GetZoomSpeed() const { return zoom_speed; }
	inline void SetZoomSpeed(const float& z) { zoom_speed = z; }

	void Focus(const Model* model);

private:
	void InitFrustum();

	inline void SetPos(const float3& newpos) { frustum->SetPos(newpos); }
	inline void SetPos(const float& x, const float& y, const float& z) { frustum->SetPos(float3(x, y, z)); }

	inline void Translate(const float3& translation) { SetPos(frustum->Pos() + translation); }
	inline void Translate(const float& x, const float& y, const float& z) { SetPos(frustum->Pos() + float3(x, y, z)); }

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