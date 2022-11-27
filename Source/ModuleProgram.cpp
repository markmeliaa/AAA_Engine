#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleEditor.h"

#include "GL/glew.h"

ModuleProgram::ModuleProgram()
{
}

ModuleProgram::~ModuleProgram()
{
}

bool ModuleProgram::Start()
{
	program = CreateProgram();
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
		data = (char*)malloc(static_cast<size_t>(size) + 1);
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
	unsigned vertex = CompileShader(GL_VERTEX_SHADER, LoadShaderSource("default_vertex.glsl"));
	unsigned fragment = CompileShader(GL_FRAGMENT_SHADER, LoadShaderSource("default_fragment.glsl"));

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