#ifndef __ModuleTimer_H__
#define __ModuleTimer_H__

#include "Module.h"

class ModuleTimer : public Module
{
public:
	ModuleTimer();
	~ModuleTimer() override;

	update_status PreUpdate() override;
	update_status Update() override;
	update_status PostUpdate() override;
	bool CleanUp() override;

	float fps_log[85] = {};
	float milisec_log[85] = {};

	float GetDeltaTime() const { return delta_time; }

private:
	unsigned time_before_loop = 0;
	unsigned time_after_loop = 0;
	float delta_time = 0.0f;

	void CalculateFpsPerFrame();
	void CalculateMilisecPerFrame();
};

#endif // __ModuleTimer_H__