#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleProgram;
class ModuleRenderExercise;
class ModuleDebugDraw;
class ModuleEditor;

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
	ModuleRenderExercise* render_ex = nullptr;
	ModuleDebugDraw* draw = nullptr;
	ModuleEditor* editor = nullptr;

	void RequestBrowser(const char* url);

private:

	std::list<Module*> modules;
};

extern Application* App;