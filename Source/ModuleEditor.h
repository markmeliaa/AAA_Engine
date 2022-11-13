#pragma once
#include "Module.h"
#include "Globals.h"
#include <vector>

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

	void SetMaxFps(float fps);
	float GetMaxFps() const;

private:
	int loops = 0;
	float max_fps = 60;
	float fps_log[85] = {};
	float milisec_log[85] = {};

	bool scroll_log;

	void DrawMainMenu(bool& demo_w) const;
	void DrawLog(bool& log_w, bool& scroll) const;
	void DrawAbout(bool& about_w) const;
	void DrawConfig(bool& config_w);
};