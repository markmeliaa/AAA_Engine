#pragma once
#include "Globals.h"
#include "Application.h"

class Panel
{
public:

	Panel()
	{
	}

	virtual void Draw() = 0;

	virtual bool CleanUp()
	{
		return true;
	}

	bool getFocused() const { return focused; }
	void setFocused(const bool& f) { focused = f; }

	bool visible = true;

private:
	bool focused = false;
};
