#pragma once
#include "Module.h"
#include "Globals.h"
#include <vector>

#define NUMBERWINDOWS 3

class ModuleEditor : public Module
{
public:
	ModuleEditor();
	~ModuleEditor();

	bool Init() override;
	bool Start() override;
	update_status PreUpdate() override;
	update_status Update() override;
	update_status PostUpdate() override;
	bool CleanUp() override;

	std::vector<const char*> log;

	void SetMaxFps(const float& fps);
	float GetMaxFps() const;

	bool IsAnyWindowsFocused() const;

private:
	int loops = 0;
	float max_fps = 60;
	float fps_log[85] = {};
	float milisec_log[85] = {};

	bool log_w = true;
	bool config_w = true;
	bool about_w = true;

	bool WindowsFocused[NUMBERWINDOWS] = { false };

	void DrawMainMenu();
	void DrawLog();
	void DrawAbout();
	void DrawConfig();
};