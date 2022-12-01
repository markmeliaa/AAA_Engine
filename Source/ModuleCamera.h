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

	float4x4 GetViewMatrix() const;
	float4x4 GetProjMatrix() const;
	float4x4 GetModelMatrix() const;

	float3 GetModelTrans() const;
	void SetModelTrans(const float3& t);

	float GetModelRotX() const;
	float GetModelRotY() const;
	float GetModelRotZ() const;
	void SetModelRot(const float& x, const float& y, const float& z);

	float3 GetModelScale() const;
	void SetModelScale(const float3& s);

	float GetNearPlane() const;
	void SetNearPlane(const float& np);
	float GetFarPlane() const;
	void SetFarPlane(const float& fp);

	float GetMoveSpeed() const;
	void SetMoveSpeed(const float& m);
	float GetRotSpeed() const;
	void SetRotSpeed(const float& r);
	float GetZoomSpeed() const;
	void SetZoomSpeed(const float& z);

private:
	void SetUpFrustum();

	void SetPos(const float3& newpos);
	void SetPos(const float& x, const float& y, const float& z);

	void Translate(const float3& translation);
	void Translate(const float& x, const float& y, const float& z);
	void Rotate(const float3x3& rotationDeltaMatrix);
	void Focus(const Model* model);
	void Orbit(const Model* model, const float& move_speed, const float& rotate_speed);

	float aspectRatio = 0.0f;
	Frustum* frustum;
	float4x4 model, view, proj;

	float3 model_trans, model_scale;
	float rotX, rotY, rotZ = 0.0f;
	float4x4 model_rot;

	float move_speed = 3.0f;
	float rotate_speed = 25.0f;
	float zoom_speed = 3.0f;
};

#endif // __ModuleCamera_H__