#pragma warning( disable : 4267 )

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleProgram.h"
#include "ModuleEditor.h"
#include "ModuleDebugDraw.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"
#include "ModuleRender.h"

#include <SDL_timer.h>

using namespace std;

Application::Application()
{
	// Order matters: they will Init/start/update in this order
	modules.push_back(window = new ModuleWindow());
	modules.push_back(input = new ModuleInput());

	modules.push_back(program = new ModuleProgram());
	modules.push_back(editor = new ModuleEditor());
	modules.push_back(draw = new ModuleDebugDraw());
	modules.push_back(camera = new ModuleCamera());
	modules.push_back(texture = new ModuleTexture());

	modules.push_back(renderer = new ModuleRender());
}

Application::~Application()
{
	for(int i = 0; i < modules.size(); ++i)
    {
        delete modules[i];
    }
}

bool Application::Init()
{
	bool ret = true;

	for(int i = 0; i < modules.size(); ++i)
		ret = modules[i]->Init();

	D_LOG("Application Start --------------")
	App->editor->log.emplace_back("-------------- Application Start --------------");

	for (int i = 0; i < modules.size(); ++i)
		ret = modules[i]->Start();

	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	unsigned now_time = SDL_GetTicks();
	if (SDL_TICKS_PASSED(now_time, previous_time))
	{
		unsigned delta_ms = now_time - previous_time;
		delta_time = delta_ms / 1000.0f;

		for (int i = 0; i < modules.size() && ret == UPDATE_CONTINUE; ++i)
			ret = modules[i]->PreUpdate();

		for (int i = 0; i < modules.size() && ret == UPDATE_CONTINUE; ++i)
			ret = modules[i]->Update();

		for (int i = 0; i < modules.size() && ret == UPDATE_CONTINUE; ++i)
			ret = modules[i]->PostUpdate();
	}
	previous_time = now_time;

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(int i = modules.size() - 1; i >= 0; --i)
		ret = modules[i]->CleanUp();

	return ret;
}

void Application::RequestBrowser(const char* url)
{
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}

float Application::GetDeltaTime() const
{
	return delta_time;
}