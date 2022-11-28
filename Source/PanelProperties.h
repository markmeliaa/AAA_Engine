#ifndef __PanelProperties_H__
#define __PanelProperties_H__

#include "Panel.h"

class PanelProperties : public Panel
{
public:
	PanelProperties();
	~PanelProperties();

	void Draw() override;
};

#endif // __PanelProperties_H__