#ifndef __ModuleProgram_H__
#define __ModuleProgram_H__

#include "Module.h"
#include "Globals.h"

class ModuleProgram : public Module
{
public:
	ModuleProgram();
	~ModuleProgram();

	bool Start() override;

	unsigned int program = 0;

private:
	char* LoadShaderSource(const char* shader_file_name);
	unsigned CompileShader(const unsigned& type, const char* source);
	unsigned CreateProgram();
};

#endif // __ModuleProgram_H__