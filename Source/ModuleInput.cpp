#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleEditor.h"
#include "ModuleCamera.h"

#include "lib/SDL/include/SDL.h"
#include "lib/imgui-docking/imgui.h"
#include "lib/imgui-docking/imgui_impl_sdl.h"
#include "lib/imgui-docking/imgui_impl_opengl3.h"

ModuleInput::ModuleInput()
{
}

// Destructor
ModuleInput::~ModuleInput()
{
}

// Called before render is available
bool ModuleInput::Init()
{
	D_LOG("Init SDL input event system");
	App->editor->log.emplace_back("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		D_LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		//App->editor->log.emplace_back("SDL_EVENTS could not initialize! SDL_Error:");
		//App->editor->log.emplace_back(SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::Update()
{
    SDL_Event sdlEvent;

    while (SDL_PollEvent(&sdlEvent) != 0)
    {
        switch (sdlEvent.type)
        {
            case SDL_QUIT:
                return UPDATE_STOP;
            case SDL_WINDOWEVENT:
                if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED || sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                    App->renderer->WindowResized(sdlEvent.window.data1, sdlEvent.window.data2);
                break;
        }
    }

    keyboard = SDL_GetKeyboardState(NULL);

	// Exit with ESC
	if (keyboard[SDL_SCANCODE_ESCAPE])
	{
		//App->CleanUp();
		return UPDATE_STOP;
	}

	if (keyboard[SDL_SCANCODE_RIGHT])
	{
		//return UPDATE_STOP;
	}

	ImGui_ImplSDL2_ProcessEvent(&sdlEvent);

    return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	D_LOG("Quitting SDL input event subsystem");
	App->editor->log.emplace_back("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}
