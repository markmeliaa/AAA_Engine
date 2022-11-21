#pragma once

#include <list>
#include <vector>
#include "Module.h"
#include "Globals.h"

#include "Panel.h"

class PanelConsole;
class PanelAbout;
class PanelConfig;

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

	void SetMaxFps(const int& fps);
	int GetMaxFps() const;

	bool IsAnyWindowsFocused();

	PanelConsole* console = nullptr;
	PanelAbout* about = nullptr;
	PanelConfig* config = nullptr;

	float fps_log[85] = {};
	float milisec_log[85] = {};

private:
	int loops = 0;
	float max_fps = 60;

	std::list<Panel*> panels;

	bool log_w = true;
	bool config_w = true;
	bool about_w = true;

	void DrawMainMenu();
};