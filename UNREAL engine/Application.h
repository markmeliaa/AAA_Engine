#ifndef __Application_H__
#define __Application_H__

#include "Globals.h"

#include <vector>

class Module;
class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleProgram;
class ModuleDebugDraw;
class ModuleEditor;
class ModuleCamera;
class ModuleTexture;
class ModuleTimer;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleProgram* program = nullptr;
	ModuleDebugDraw* draw = nullptr;
	ModuleEditor* editor = nullptr;
	ModuleCamera* camera = nullptr;
	ModuleTexture* texture = nullptr;
	ModuleTimer* timer = nullptr;

	void RequestBrowser(const char* url);

private:
	std::vector<Module*> modules = {};
};

extern Application* App;

#endif // __Application_H__