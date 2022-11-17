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

	keyboard = SDL_GetKeyboardState(NULL);

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate()
{
    SDL_Event sdlEvent;

    while (SDL_PollEvent(&sdlEvent) != 0)
    {
        switch (sdlEvent.type)
        {
            case SDL_QUIT:
                return UPDATE_STOP;
            case SDL_WINDOWEVENT:
				if (sdlEvent.window.windowID == SDL_GetWindowID(App->window->window))
				{
					switch (sdlEvent.window.event)
					{
						case SDL_WINDOWEVENT_RESIZED:
							App->editor->log.emplace_back("Windows resized!!!");
							App->window->SetWindowSize(sdlEvent.window.data1, sdlEvent.window.data2);
							break;
						case SDL_WINDOWEVENT_CLOSE:
							return UPDATE_STOP;
					}
				}

				break;

			case SDL_MOUSEBUTTONDOWN:
				if (!App->editor->IsAnyWindowsFocused() && mouseButtons[sdlEvent.button.button] != MouseButtonStates::DOWN)
				{
					mouseButtons[sdlEvent.button.button] = MouseButtonStates::DOWN;
				}
				break;

			case SDL_MOUSEBUTTONUP:
				if (mouseButtons[sdlEvent.button.button] != MouseButtonStates::UP)
				{
					mouseButtons[sdlEvent.button.button] = MouseButtonStates::UP;
				}
				break;
        }
    }

	// Exit with ESC
	if (keyboard[SDL_SCANCODE_ESCAPE])
	{
		return UPDATE_STOP;
	}

	ImGui_ImplSDL2_ProcessEvent(&sdlEvent);

	int mouseX = 0;
	int mouseY = 0;
	SDL_GetGlobalMouseState(&mouseX, &mouseY);
	mouseCurrentPos.x = (float)mouseX - mouseLastPos.x;
	mouseCurrentPos.y = (float)mouseY - mouseLastPos.y;

	mouseLastPos.x = (float)mouseX;
	mouseLastPos.y = (float)mouseY;

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

bool ModuleInput::GetKey(const int& key) const
{
	return keyboard[key];
}

bool ModuleInput::GetMouseButton(const int& button) const
{
	return mouseButtons[button] == MouseButtonStates::DOWN;
}

float2 ModuleInput::GetMouseInput() const
{
	return mouseCurrentPos;
}