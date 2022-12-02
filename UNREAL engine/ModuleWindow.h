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
	void SetWindowResizable(const bool& resize) const;
	void SetWindowBorderless(const bool& borders) const;

	void SetWindowBrightness(const float& brightness) const;
	float GetWindowBrightness() const;

	int getCurrentWidth() const;
	void setCurrentWidth(const int& w);

	int getCurrentHeight() const;
	void setCurrentHeight(const int& h);

	int getMaxWindowsWidth() const;
	int getMaxWindowsHeight() const;

public:
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