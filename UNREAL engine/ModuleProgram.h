#ifndef __ModuleProgram_H__
#define __ModuleProgram_H__

#include "Module.h"

class ModuleProgram : public Module
{
public:
	ModuleProgram();
	~ModuleProgram() override;

	bool Start() override;

	unsigned int GetProgram() { return program; }

private:
	char* LoadShaderSource(const char* shader_file_name);
	unsigned CompileShader(unsigned type, const char* source);
	unsigned CreateProgram();

	unsigned int program = 0;
};

#endif // __ModuleProgram_H__