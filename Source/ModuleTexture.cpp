#include "Globals.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "ModuleEditor.h"

#include "DirectXTex.h"

ModuleTexture::ModuleTexture()
{
}

ModuleTexture::~ModuleTexture()
{
}

bool ModuleTexture::Init()
{
	D_LOG("Load the texture");
	App->editor->log.emplace_back("Load the texture");

	return true;
}

bool ModuleTexture::Start()
{
	return true;
}

update_status ModuleTexture::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleTexture::Update()
{
	return UPDATE_CONTINUE;
}

update_status ModuleTexture::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleTexture::CleanUp()
{
	return true;
}
