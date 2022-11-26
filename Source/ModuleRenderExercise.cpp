#pragma once
#pragma warning( disable : 4267 )
#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleDebugDraw.h"
#include "ModuleRenderExercise.h"
#include "ModuleEditor.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "ModuleTexture.h"

#include "GL/glew.h"
#include "DirectXTex.h"

ModuleRenderExercise::ModuleRenderExercise()
{
}

ModuleRenderExercise::~ModuleRenderExercise()
{
}

bool ModuleRenderExercise::Init()
{
	return true;
}

bool ModuleRenderExercise::Start()
{
	D_LOG("Creating and linking program");
	App->editor->log.emplace_back("Creating and linking program");

	model = new Model("Bakerhouse.fbx");

	return true;
}

update_status ModuleRenderExercise::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::Update()
{
	model->Draw();

	App->draw->Draw(App->camera->GetViewMatrix(), App->camera->GetProjMatrix(), App->window->getCurrentWidth(), App->window->getCurrentHeight());

	return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp()
{
	D_LOG("Deleting buffer objects");
	App->editor->log.emplace_back("Deleting buffer objects");

	delete model;
	return true;
}