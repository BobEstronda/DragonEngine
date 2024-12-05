#pragma once

#include <SDL.h>
#include <memory>

namespace DragonUtil 
{
	struct SDL_Destroyer
	{
		void operator()(SDL_Window* window) const;
		void operator()(SDL_GameController* controller) const;
		void operator()(SDL_Cursor* cursor) const;
	};
}

typedef std::shared_ptr<SDL_GameController> Controller;
static Controller make_shared_controller(SDL_GameController* controller);

typedef std::shared_ptr<SDL_Cursor> Cursor;
static Cursor make_shared_cursor(SDL_GameController* cursor);

typedef std::unique_ptr<SDL_Window, DragonUtil::SDL_Destroyer> WindowPtr;