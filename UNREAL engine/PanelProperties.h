#ifndef __PanelProperties_H__
#define __PanelProperties_H__

#include "Panel.h"
#include <dxgi1_4.h>
#include <GL/glew.h>

class PanelProperties : public Panel
{
public:
	PanelProperties();
	~PanelProperties();

	void Draw() override;

private:
	IDXGIFactory2* pDXGI_factory = nullptr;
	IDXGIAdapter3* pDXGI_adapter = nullptr;
	DXGI_QUERY_VIDEO_MEMORY_INFO local_video_memory_info;

	GLuint current_texture;
};

#endif // __PanelProperties_H__