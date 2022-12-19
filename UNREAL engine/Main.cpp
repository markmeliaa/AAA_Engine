#include <stdlib.h>
#include "Application.h"
#include "Globals.h"
#include "ModuleRender.h"
#include "ModuleEditor.h"

#include <SDL.h>
#pragma comment( lib, "lib/SDL/lib/x64/SDL2.lib" )
#pragma comment( lib, "lib/SDL/lib/x64/SDL2main.lib" )

enum main_states
{
	MAIN_CREATION,  
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

Application* App = nullptr;

int main(int argc, char ** argv)
{
	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:

			App = new Application();
			D_LOG("Application Creation --------------");
			App->editor->log.emplace_back("-------------- Application Creation --------------");
			state = MAIN_START;
			break;

		case MAIN_START:

			D_LOG("Application Init --------------");
			App->editor->log.emplace_back("-------------- Application Init --------------");
			if (!App->Init())
			{
				D_LOG("Application Init exits with error -----");
				App->editor->log.emplace_back("----- Application Init exits with error -----");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				D_LOG("Application Update --------------");
				App->editor->log.emplace_back("-------------- Engine ready to be used! --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				D_LOG("Application Update exits with error -----");
				App->editor->log.emplace_back("----- Application Update exits with error -----");
				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
			break;

		case MAIN_FINISH:

			D_LOG("Application CleanUp --------------");
			App->editor->log.emplace_back("-------------- Application CleanUp --------------");

			if (!App->CleanUp())
			{
				D_LOG("Application CleanUp exits with error -----");
				App->editor->log.emplace_back("----- Application CleanUp exits with error -----");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}

	}

	delete App;
	D_LOG("Bye :)\n");
	return main_return;
}
