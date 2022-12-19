#ifndef __ModuleInput_H__
#define __ModuleInput_H__

#include "Module.h"

#include <Math/float2.h>

#define MOUSEBUTTONS 5

enum MouseButtonStates
{
	REST,
	UP,
	DOWN
};

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput() override;

	bool Init() override;
	update_status PreUpdate() override;
	bool CleanUp() override;

	bool GetKey(int key) const { return keyboard[key]; }

	bool GetMouseButton(int button) const { return mouseButtons[button] == MouseButtonStates::DOWN; }
	const float2& GetMouseInput() const { return mouseCurrentPos; }
	const float2& GetMouseWheelInput() const { return mouseWheel; }

	bool GetMouseWheelMoving() const { return mouseWheelMoving; }
	void SetMouseWheel(bool wheel) { mouseWheelMoving = wheel; }

private:
	const uint8_t* keyboard = NULL;
	MouseButtonStates mouseButtons[MOUSEBUTTONS] = { MouseButtonStates::REST };

	float2 mouseLastPos = { 0, 0 };
	float2 mouseCurrentPos = { 0, 0 };

	bool mouseWheelMoving = false;
	float2 mouseWheel = { 0,0 };
};

#endif // __ModuleInput_H__