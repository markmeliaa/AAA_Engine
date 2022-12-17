#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleEditor.h"

ModuleWindow::ModuleWindow()
{
	currentWidth = BASE_SCREEN_WIDTH;
	currentHeight = BASE_SCREEN_HEIGHT;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	D_LOG("Init SDL window & surface");
	App->editor->log.emplace_back("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		D_LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		App->editor->log.emplace_back("SDL_VIDEO could not initialize! SDL_Error");
		App->editor->log.emplace_back(SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		int width = BASE_SCREEN_WIDTH;
		int height = BASE_SCREEN_HEIGHT;
		Uint32 flags = SDL_WINDOW_SHOWN |  SDL_WINDOW_OPENGL;

		if(FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		else if (BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		else if (RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		else if (FULLSCREEN_DSKTP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			D_LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			App->editor->log.emplace_back("Window could not be created! SDL_Error:");
			App->editor->log.emplace_back(SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}

		SDL_GetCurrentDisplayMode(0, &max_win_size);
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	D_LOG("Destroying SDL window and quitting all SDL systems");
	App->editor->log.emplace_back("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetWindowSize(const int& width, const int& height)
{
	int display_index = SDL_GetWindowDisplayIndex(window);
	SDL_SetWindowSize(window, width, height);
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED_DISPLAY(display_index), SDL_WINDOWPOS_CENTERED_DISPLAY(display_index));

	SetCurrentWidth(width);
	SetCurrentHeight(height);
}