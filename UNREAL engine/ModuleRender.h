#ifndef __ModuleRender_H__
#define __ModuleRender_H__

#include "Module.h"
#include "Globals.h"
#include "Model.h"

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init() override;
	bool Start() override;
	update_status PreUpdate() override;
	update_status Update() override;
	update_status PostUpdate() override;
	bool CleanUp() override;

	void* GetContext() const { return context; }
	Model* GetModel() const { return model; }
	void LoadNewModel(const char* load_file);

	float background_color[3] = { 0.1f, 0.1f, 0.1f };

private:
	void* context = nullptr;
	Model* model = nullptr;
};

#endif // __ModuleRender_H__