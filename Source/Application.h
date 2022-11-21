#pragma once

#include <list>
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
class ModuleCamera;

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
	ModuleCamera* camera = nullptr;

	void RequestBrowser(const char* url);

	float GetDeltaTime() const;

private:
	std::list<Module*> modules;

	unsigned previous_time = 0;
	float delta_time = 0.0f;
};

extern Application* App;