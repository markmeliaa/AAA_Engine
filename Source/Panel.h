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

	bool getFocused() const
	{
		return this->focused;
	}

	void setFocused(const bool& f)
	{
		this->focused = f;
	}

	bool visible = true;

private:
	bool focused = false;
};
