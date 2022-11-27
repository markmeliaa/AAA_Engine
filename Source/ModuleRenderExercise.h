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

	bool Start() override;
	update_status Update() override;
	bool CleanUp() override;

private:
	Model* model;
};

#endif // __ModuleRenderExercise_H__