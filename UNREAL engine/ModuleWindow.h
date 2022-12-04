#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "Globals.h"

#include <SDL.h>

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	// Called before quitting
	bool Init() override;

	// Called before quitting
	bool CleanUp() override;

	void SetWindowSize(const int& width, const int& height);
	inline void SetWindowResizable(const bool& resize) const { resize ? SDL_SetWindowResizable(window, SDL_TRUE) : SDL_SetWindowResizable(window, SDL_FALSE); }
	inline void SetWindowBorderless(const bool& borders) const { borders ? SDL_SetWindowBordered(window, SDL_FALSE) : SDL_SetWindowBordered(window, SDL_TRUE); }
	inline void SetWindowFullscreen(const bool& full_scr) const { full_scr ? SDL_SetWindowFullscreen(window, SDL_TRUE) : SDL_SetWindowFullscreen(window, SDL_FALSE); }

	inline int GetCurrentWidth() const { return currentWidth; }
	inline void SetCurrentWidth(const int& w) { currentWidth = w; }
	inline int GetCurrentHeight() const { return currentHeight; }
	inline void SetCurrentHeight(const int& h) { currentHeight = h; }

	inline int GetMaxWindowsWidth() const { return max_win_size.w; }
	inline int GetMaxWindowsHeight() const { return max_win_size.h; }

	inline void SetWindowBrightness(const float& brightness) const { SDL_SetWindowBrightness(window, brightness); }
	inline float GetWindowBrightness() const { return SDL_GetWindowBrightness(window); }

	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;

private:
	int currentWidth;
	int currentHeight;

	SDL_DisplayMode max_win_size;
};

#endif // __ModuleWindow_H__