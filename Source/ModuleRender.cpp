#pragma once
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleEditor.h"
#include "ModuleDebugDraw.h"
#include "ModuleCamera.h"

#include <SDL.h>
#include <GL/glew.h>

ModuleRender::ModuleRender()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); // desired version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);	// we want a double buffer
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);		// we want to have a depth buffer with 24 bits
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);	// we want to have a stencil buffer with 8 bits
}

// Destructor
ModuleRender::~ModuleRender()
{
}

// Called before render is available
bool ModuleRender::Init()
{
	D_LOG("Creating Renderer context");
	App->editor->log.emplace_back("Creating Renderer context");
	context = SDL_GL_CreateContext(App->window->window);

	GLenum err = glewInit();
	// … check for errors
	D_LOG("Using Glew %s", glewGetString(GLEW_VERSION));
	App->editor->log.emplace_back("Using Glew 2.1.0");
	// Should be 2.0

	D_LOG("Vendor: %s", glGetString(GL_VENDOR));
	App->editor->log.emplace_back("Vendor: Intel");
	D_LOG("Renderer: %s", glGetString(GL_RENDERER));
	App->editor->log.emplace_back("Renderer: Intel(R) Iris(R) Xe Graphics");
	D_LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	App->editor->log.emplace_back("OpenGL version supported 4.6.0 - Build 30.0.101.1029");
	D_LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	App->editor->log.emplace_back("GLSL: 4.60 - Build 30.0.101.1029");
	
	glEnable(GL_DEPTH_TEST);	// Enable depth test
	glEnable(GL_CULL_FACE);		// Enable cull backward faces
	glFrontFace(GL_CCW);		// Front faces will be counter clockwise

	// Cap the FPS to 60
	SDL_GL_SetSwapInterval(1);

	return true;
}

bool ModuleRender::Start()
{
	D_LOG("***** ASSIMP PROCESS START *****");
	App->editor->log.emplace_back("***** ASSIMP PROCESS START *****");

	D_LOG("Load the inital model");
	App->editor->log.emplace_back("Load the initial model");
	model = new Model("LinuxPenguin.obj");

	D_LOG("***** ASSIMP PROCESS END *****");
	App->editor->log.emplace_back("***** ASSIMP PROCESS END *****");

	return true;
}

update_status ModuleRender::PreUpdate()
{
	int w, h;
	SDL_GetWindowSize(App->window->window, &w, &h);

	glViewport(0, 0, w, h);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}

update_status ModuleRender::Update()
{
	model->Draw();

	App->draw->Draw(App->camera->GetViewMatrix(), App->camera->GetProjMatrix(), App->window->getCurrentWidth(), App->window->getCurrentHeight());

	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	D_LOG("Destroying renderer");
	App->editor->log.emplace_back("Destroying renderer");

	//Destroy window
	SDL_GL_DeleteContext(context);

	delete model;
	return true;
}

void* ModuleRender::GetContext() const
{
	return context;
}

Model* ModuleRender::GetModel() const
{
	return model;
}