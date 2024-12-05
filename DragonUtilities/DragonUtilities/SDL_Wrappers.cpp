#include "SDL_Wrappers.h"
#include <iostream>

void DragonUtil::SDL_Destroyer::operator()(SDL_Window* window) const
{
    if (window)
    {
        SDL_DestroyWindow(window);
        std::cout << "Destroyed SDL Window\n";
    }
}

void DragonUtil::SDL_Destroyer::operator()(SDL_GameController* controller) const
{
}

void DragonUtil::SDL_Destroyer::operator()(SDL_Cursor* cursor) const
{
}

Controller make_shared_controller(SDL_GameController* controller)
{
    return Controller();
}

Cursor make_shared_cursor(SDL_GameController* cursor)
{
    return Cursor();
}
