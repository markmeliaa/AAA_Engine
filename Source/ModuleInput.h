#pragma once
#include "Module.h"
#include "Globals.h"

#include "lib/MathGeoLib/Math/float2.h"

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
	//bool Start() override;
	update_status PreUpdate() override;
	//update_status Update() override;
	bool CleanUp() override;

	bool GetKey(const int& key) const;
	bool GetMouseButton(const int& button) const;
	float2 GetMouseInput() const;

private:
	const Uint8 *keyboard = NULL;
	MouseButtonStates mouseButtons[MOUSEBUTTONS] = { MouseButtonStates::REST };

	float2 mouseLastPos = { 0, 0 };
	float2 mouseCurrentPos = { 0, 0 };
};