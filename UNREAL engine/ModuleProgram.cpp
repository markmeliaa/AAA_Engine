#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleEditor.h"

#include <GL/glew.h>
#include <assert.h>

ModuleProgram::ModuleProgram()
{
}

ModuleProgram::~ModuleProgram()
{
	glDeleteProgram(program);
}

bool ModuleProgram::Start()
{
	program = CreateProgram();
	return true;
}

char* ModuleProgram::LoadShaderSource(const char* shader_file_name)
{
	assert(shader_file_name != nullptr);

	char* data = nullptr;
	FILE* file = nullptr;

	fopen_s(&file, shader_file_name, "rb");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		data = (char*)malloc(static_cast<size_t>(size) + 1);
		fseek(file, 0, SEEK_SET);
		fread(data, 1, size, file);
		data[size] = 0;
		fclose(file);
	}
	return data;
}

unsigned ModuleProgram::CompileShader(unsigned type, const char* shader_source)
{
	assert(shader_source != nullptr);

	unsigned shader_id = glCreateShader(type);
	glShaderSource(shader_id, 1, &shader_source, 0);
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
			App->editor->log.emplace_back("Shader Log Info:");
			App->editor->log.emplace_back(info);
			free(info);
		}
	}
	return shader_id;
}

unsigned ModuleProgram::CreateProgram()
{
	D_LOG("Load shaders from .glsl files and create shader objects");
	App->editor->log.emplace_back("Load shaders from .glsl files and create shader objects");

	const char* vertex_file = LoadShaderSource("Shaders/default_vertex.glsl");
	const char* fragment_file = LoadShaderSource("Shaders/default_fragment.glsl");

	unsigned vertex = CompileShader(GL_VERTEX_SHADER, vertex_file);
	unsigned fragment = CompileShader(GL_FRAGMENT_SHADER, fragment_file);

	delete vertex_file;
	delete fragment_file;

	D_LOG("Create and link program");
	App->editor->log.emplace_back("Create and link program");
	unsigned programId = glCreateProgram();
	glAttachShader(programId, vertex);
	glAttachShader(programId, fragment);
	glLinkProgram(programId);
	int res;
	glGetProgramiv(programId, GL_LINK_STATUS, &res);
	if (res == GL_FALSE)
	{
		int len = 0;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			int written = 0;
			char* info = (char*)malloc(len);
			glGetProgramInfoLog(programId, len, &written, info);
			D_LOG("Program Log Info: %s", info);
			free(info);
		}
	}

	D_LOG("Delete shaders once linked");
	App->editor->log.emplace_back("Delete shaders once linked");
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return programId;
}