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

	float aspect = 0;
	float4x4 model, view, proj;
	float3x3 rotationDeltaMatrix;

private:
	void SetUpFrustum();
	Frustum frustum;
};