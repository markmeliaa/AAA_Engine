#ifndef __PanelConfig_H__
#define __PanelConfig_H__

#include "Panel.h"

class PanelConfig : public Panel
{
public:
	PanelConfig();
	~PanelConfig();

	void Draw() override;
	//bool CleanUp() override;
};

#endif // __PanelConfig_H__