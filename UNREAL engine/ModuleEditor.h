#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

#include "Module.h"
#include "Globals.h"
#include "Panel.h"

#include <list>
#include <vector>

class PanelAbout;
class PanelConsole;
class PanelConfig;
class PanelProperties;

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

	PanelAbout* about = nullptr;
	PanelConsole* console = nullptr;
	PanelConfig* config = nullptr;
	PanelProperties* props = nullptr;

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

	update_status DrawMainMenu();
};

#endif // __ModuleEditor_H__