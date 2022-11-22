#pragma once
#include "Panel.h"

class PanelAbout : public Panel
{
public:
	PanelAbout();
	~PanelAbout();

	void Draw() override;
	//bool CleanUp() override;
};