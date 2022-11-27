#pragma once
#include "Application.h"
#include "ModuleRenderExercise.h"
#include "ModuleDebugDraw.h"
#include "ModuleEditor.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"

#include <GL/glew.h>
#include <DirectXTex.h>

ModuleRenderExercise::ModuleRenderExercise()
{
}

ModuleRenderExercise::~ModuleRenderExercise()
{
}

bool ModuleRenderExercise::Start()
{
	D_LOG("Load the model");
	App->editor->log.emplace_back("Load the model");
	model = new Model("Bakerhouse.fbx");

	return true;
}

update_status ModuleRenderExercise::Update()
{
	model->Draw();

	App->draw->Draw(App->camera->GetViewMatrix(), App->camera->GetProjMatrix(), App->window->getCurrentWidth(), App->window->getCurrentHeight());

	return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp()
{
	delete model;
	return true;
}