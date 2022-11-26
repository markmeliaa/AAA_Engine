#ifndef __ModuleRenderExercise_H__
#define __ModuleRenderExercise_H__

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
	unsigned int ebo = 0;
	unsigned int vao = 0;

	unsigned CreateProgram(unsigned vtx_shader, unsigned frg_shader);

	void CreateQuadBuffers();
	void RenderQuad(unsigned program);

	void DestroyVBO(unsigned vbo);
	void DestroyEBO(unsigned ebo);
	void DestroyVAO(unsigned vao);
};

#endif // __ModuleRenderExercise_H__