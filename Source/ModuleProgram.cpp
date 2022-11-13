#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleEditor.h"

#include "lib/glew-2.1.0/include/GL/glew.h"

ModuleProgram::ModuleProgram()
{
}

ModuleProgram::~ModuleProgram()
{
}

bool ModuleProgram::Start()
{
	D_LOG("Loading from shader files");
	App->editor->log.emplace_back("Loading from shader files");
	vertex_file = LoadShaderSource("default_vertex.glsl");
	fragment_file = LoadShaderSource("default_fragment.glsl");

	D_LOG("Creating shader objects");
	App->editor->log.emplace_back("Creating shader objects");
	vertex_shader = CompileShader(GL_VERTEX_SHADER, vertex_file);
	fragment_shader = CompileShader(GL_FRAGMENT_SHADER, fragment_file);

	return true;
}

char* ModuleProgram::LoadShaderSource(const char* shader_file_name)
{
	char* data = nullptr;
	FILE* file = nullptr;

	fopen_s(&file, shader_file_name, "rb");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		data = (char*)malloc(size + 1);
		fseek(file, 0, SEEK_SET);
		fread(data, 1, size, file);
		data[size] = 0;
		fclose(file);
	}
	return data;
}

unsigned ModuleProgram::CompileShader(unsigned type, const char* source)
{
	unsigned shader_id = glCreateShader(type);
	glShaderSource(shader_id, 1, &source, 0);
	glCompileShader(shader_id);

	int res = GL_FALSE;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE)
	{
		int len = 0;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			int written = 0;
			char* info = (char*)malloc(len);
			glGetShaderInfoLog(shader_id, len, &written, info);
			D_LOG("Shader Log Info: %s", info);
			//App->editor->log.emplace_back("Shader Log Info:");
			//App->editor->log.emplace_back(info);
			free(info);
		}
	}
	return shader_id;
}