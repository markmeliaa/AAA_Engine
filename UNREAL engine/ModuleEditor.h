#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

#include "Module.h"

#include <vector>

class Panel;
class PanelAbout;
class PanelConsole;
class PanelConfig;
class PanelProperties;

class ModuleEditor : public Module
{
public:
	ModuleEditor();
	~ModuleEditor() override;

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

	void CalculateFpsPerFrame();
	void CalculateMilisecPerFrame();
	bool IsAnyWindowsFocused();

private:
	std::vector<Panel*> panels;

	update_status DrawMainMenu();
	void SetUpEditorColors();
};

#endif // __ModuleEditor_H__