#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

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

	PanelConsole* console = nullptr;
	PanelAbout* about = nullptr;
	PanelConfig* config = nullptr;

	float fps_log[85] = {};
	float milisec_log[85] = {};

	void SetMaxFps(const int& fps);
	int GetMaxFps() const;

	void GetFpsPerFrame();
	void GetMilisecPerFrame();
	bool IsAnyWindowsFocused();

private:
	std::list<Panel*> panels;
	int limited_fps = 60;

	void DrawMainMenu();
};

#endif // __ModuleEditor_H__