#pragma once
#include "Module.h"
#include "Globals.h"

typedef unsigned __int8 Uint8;

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init() override;
	//bool Start() override;
	update_status PreUpdate() override;
	//update_status Update() override;
	bool CleanUp() override;

	bool KeyPressed(const int& key) const;

private:
	const Uint8 *keyboard = NULL;
};