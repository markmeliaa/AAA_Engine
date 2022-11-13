#pragma once
#include "Module.h"
#include "Globals.h"

class ModuleDummy : public Module
{
	bool Init()
	{
		D_LOG("Dummy Init!");
		return true;
	}

	bool CleanUp()
	{
		D_LOG("Dummy CleanUp!");
		return true;
	}
};