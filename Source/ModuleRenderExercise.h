#ifndef __ModuleRenderExercise_H__
#define __ModuleRenderExercise_H__

#include "Module.h"
#include "Globals.h"
#include "Model.h"

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
	Model* model;
};

#endif // __ModuleRenderExercise_H__