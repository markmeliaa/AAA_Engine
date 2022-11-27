#ifndef __ModuleDebugDraw_H__
#define __ModuleDebugDraw_H__

#include "Module.h"

#include <Math/float4x4.h>

class DDRenderInterfaceCoreGL;
class Camera;

class ModuleDebugDraw : public Module
{

public:

    ModuleDebugDraw();
    ~ModuleDebugDraw();

	//bool            Init() override;
    bool            Start() override;
	update_status   Update() override;
	bool            CleanUp() override;

    void            Draw(const float4x4& view, const float4x4& proj, unsigned width, unsigned height);
private:
    static DDRenderInterfaceCoreGL* implementation;
};

#endif // __ModuleDebugDraw_H__
