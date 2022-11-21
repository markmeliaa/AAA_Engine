#pragma once
#include "Panel.h"

class PanelConfig : public Panel
{
public:
	PanelConfig();
	~PanelConfig();

	void Draw() override;
};