#define SDL_MAIN_HANDLED 1;
#define NOMINMAX

#include "Application.h"

int main() 
{

	auto& App = DragonEditor::Application::GetInstance();
	App.Run();

	return 0;
}