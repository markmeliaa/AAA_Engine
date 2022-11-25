#ifndef __PanelConsole_H__
#define __PanelConsole_H__

#include "Panel.h"

class PanelConsole : public Panel
{
public:
	PanelConsole();
	~PanelConsole();

	void Draw() override;
	//bool CleanUp() override;
};

#endif __PanelConsole_H__