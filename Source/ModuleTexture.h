#pragma once
#include "Module.h"
#include "Globals.h"

#include "DirectXTex.h"

class ModuleTexture : public Module
{
public:
	ModuleTexture();
	~ModuleTexture();

	bool Init() override;
	bool Start() override;
	update_status PreUpdate() override;
	update_status Update() override;
	update_status PostUpdate() override;
	bool CleanUp() override;

private:

};