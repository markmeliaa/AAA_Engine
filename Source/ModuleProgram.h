#ifndef __ModuleProgram_H__
#define __ModuleProgram_H__

#include "Module.h"
#include "Globals.h"

class ModuleProgram : public Module
{
public:
	ModuleProgram();
	~ModuleProgram();

	//bool Init() override;
	bool Start() override;
	//update_status PreUpdate() override;
	//update_status Update() override;
	//update_status PostUpdate() override;
	//bool CleanUp() override;

	unsigned int vertex_shader = 0;
	unsigned int fragment_shader = 0;

private:
	char* vertex_file = nullptr;
	char* fragment_file = nullptr;

	char* LoadShaderSource(const char* shader_file_name);
	unsigned CompileShader(unsigned type, const char* source);
};

#endif // __ModuleProgram_H__