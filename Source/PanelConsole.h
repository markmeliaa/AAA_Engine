#pragma once
#include "Panel.h"

class PanelConsole : public Panel
{
public:
	PanelConsole();
	~PanelConsole();

	void Draw() override;
	//bool CleanUp() override;
};