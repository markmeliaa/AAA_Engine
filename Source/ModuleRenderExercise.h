#pragma once
#include "Module.h"
#include "Globals.h"

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

private:
	unsigned int program = 0;
	unsigned int vbo = 0;

	unsigned CreateProgram(unsigned vtx_shader, unsigned frg_shader);

	unsigned CreateQuadVBO();
	void RenderQuadVBO(unsigned vbo, unsigned program);
	void DestroyVBO(unsigned vbo);
};