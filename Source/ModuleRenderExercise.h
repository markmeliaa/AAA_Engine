#pragma once

#include "Module.h"
#include "Globals.h"
#include "lib/MathGeoLib/Geometry/Frustum.h"

class ModuleRenderExercise : public Module
{
public:
	ModuleRenderExercise();
	~ModuleRenderExercise();

	bool Init() override;
	bool Start() override;
	update_status PreUpdate() override;
	update_status Update() override;
	update_status PostUpdate() override;
	bool CleanUp() override;

	unsigned int program = 0;

private:
	unsigned int vbo = 0;
	Frustum frustum;
	float4x4 model, view, proj;
	double aspect = 0;

	unsigned CreateProgram(unsigned vtx_shader, unsigned frg_shader);

	unsigned CreateQuadVBO();
	void RenderTriangleVBO(unsigned vbo, unsigned program);
	void DestroyVBO(unsigned vbo);

	void SetUpFrustum();
};