#ifndef __Panel_H__
#define __Panel_H__

#include "Globals.h"
#include "Application.h"

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

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

protected:
	ImVec4* colors;
};

#endif // __Panel_H__