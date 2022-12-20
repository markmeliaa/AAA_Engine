#pragma warning (disable: 4244)

#include "Application.h"
#include "ModuleTimer.h"

#include <SDL_timer.h>
#include <imgui.h>

ModuleTimer::ModuleTimer()
{
}

ModuleTimer::~ModuleTimer()
{
}

update_status ModuleTimer::PreUpdate()
{
	time_before_loop = SDL_GetTicks();

	if (!SDL_TICKS_PASSED(time_before_loop, time_after_loop))
		return UPDATE_ERROR;

	CalculateFpsPerFrame(); // using ImGui
	CalculateMilisecPerFrame(); // using SDL

	return UPDATE_CONTINUE;
}

update_status ModuleTimer::Update()
{
	unsigned delta_ms = time_before_loop - time_after_loop;
	delta_time = delta_ms / 1000.0f;

	return UPDATE_CONTINUE;
}

update_status ModuleTimer::PostUpdate()
{
	time_after_loop = time_before_loop;

	return UPDATE_CONTINUE;
}

bool ModuleTimer::CleanUp()
{
	return true;
}

void ModuleTimer::CalculateFpsPerFrame()
{
	int last_checked = 0;
	for (int i = 0; i < sizeof(fps_log) / sizeof(float); ++i)
	{
		last_checked = i;
		if (fps_log[last_checked] == NULL)
		{
			fps_log[last_checked] = ImGui::GetIO().Framerate;
			break;
		}

		else if (fps_log[last_checked + 1] == NULL)
		{
			fps_log[last_checked + 1] = ImGui::GetIO().Framerate;
			break;
		}

		else
			fps_log[last_checked] = fps_log[last_checked + 1];
	}

	if (last_checked == sizeof(fps_log) / sizeof(float) - 1)
		fps_log[last_checked] = ImGui::GetIO().Framerate;
}

void ModuleTimer::CalculateMilisecPerFrame()
{
	int last_checked = 0;
	for (int i = 0; i < sizeof(milisec_log) / sizeof(float); ++i)
	{
		last_checked = i;
		if (milisec_log[last_checked] == NULL)
		{
			milisec_log[last_checked] = delta_time * 1000;
			break;
		}

		else if (milisec_log[last_checked + 1] == NULL)
		{
			milisec_log[last_checked + 1] = delta_time * 1000;
			break;
		}

		else
			milisec_log[last_checked] = milisec_log[last_checked + 1];
	}

	if (last_checked == sizeof(milisec_log) / sizeof(float) - 1)
		milisec_log[last_checked] = delta_time * 1000;
}