#ifndef __ModuleInput_H__
#define __ModuleInput_H__

#include "Module.h"
#include "Globals.h"

#include <Math/float2.h>

#define MOUSEBUTTONS 5

enum MouseButtonStates
{
	REST,
	UP,
	DOWN
};

typedef unsigned __int8 Uint8;

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init() override;
	update_status PreUpdate() override;
	bool CleanUp() override;

	inline bool GetKey(const int& key) const { return keyboard[key]; }

	inline bool GetMouseButton(const int& button) const { return mouseButtons[button] == MouseButtonStates::DOWN; }
	inline float2 GetMouseInput() const { return mouseCurrentPos; }
	inline float2 GetMouseWheelInput() const { return mouseWheel; }

	inline bool GetMouseWheelMoving() const { return mouseWheelMoving; }
	inline void SetMouseWheel(const bool& wheel) { mouseWheelMoving = wheel; }

private:
	const Uint8 *keyboard = NULL;
	MouseButtonStates mouseButtons[MOUSEBUTTONS] = { MouseButtonStates::REST };

	float2 mouseLastPos = { 0, 0 };
	float2 mouseCurrentPos = { 0, 0 };

	bool mouseWheelMoving = false;
	float2 mouseWheel = { 0,0 };
};

#endif // __ModuleInput_H__