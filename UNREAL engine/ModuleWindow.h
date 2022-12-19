#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"

#include <SDL.h>

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	~ModuleWindow() override;

	// Called before quitting
	bool Init() override;

	// Called before quitting
	bool CleanUp() override;

	void SetWindowSize(int width, int height);
	void SetWindowResizable(bool resize) const { resize ? SDL_SetWindowResizable(window, SDL_TRUE) : SDL_SetWindowResizable(window, SDL_FALSE); }
	void SetWindowBorderless(bool borders) const { borders ? SDL_SetWindowBordered(window, SDL_FALSE) : SDL_SetWindowBordered(window, SDL_TRUE); }
	void SetWindowFullscreen(bool full_scr) const { full_scr ? SDL_SetWindowFullscreen(window, SDL_TRUE) : SDL_SetWindowFullscreen(window, SDL_FALSE); }

	int GetCurrentWidth() const { return currentWidth; }
	void SetCurrentWidth(int w) { currentWidth = w; }
	int GetCurrentHeight() const { return currentHeight; }
	void SetCurrentHeight(int h) { currentHeight = h; }

	int GetMaxWindowsWidth() const { return max_win_size.w; }
	int GetMaxWindowsHeight() const { return max_win_size.h; }

	void SetWindowBrightness(float brightness) const { SDL_SetWindowBrightness(window, brightness); }
	float GetWindowBrightness() const { return SDL_GetWindowBrightness(window); }

	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;

private:
	int currentWidth = 0;
	int currentHeight = 0;

	SDL_DisplayMode max_win_size;
};

#endif // __ModuleWindow_H__