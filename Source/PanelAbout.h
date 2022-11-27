#ifndef __PanelAbout_H__
#define __PanelAbout_H__

#include "Panel.h"

class PanelAbout : public Panel
{
public:
	PanelAbout();
	~PanelAbout();

	void Draw() override;
};

#endif // __PanelAbout_H__