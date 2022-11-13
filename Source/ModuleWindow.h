#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "lib/SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	// Called before quitting
	bool Init() override;

	//bool Start() override;

	// Called before quitting
	bool CleanUp() override;

	void SetWindowSize(int width, int height);
	void SetWindowResizable(bool resize) const;
	void SetWindowBorderless(bool borders) const;

	void SetWindowBrightness(float brightness) const;
	float GetWindowBrightness() const;

	int getCurrentWidth() const;
	void setCurrentWidth(int& w);

	int getCurrentHeight() const;
	void setCurrentHeight(int& h);

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