#pragma once

#include "Logger/Logger.h"

#include <glad/glad.h>
#include <Window/Window.h>
#include <Core/ECS/Registry.h>

namespace DragonEditor
{
	class Application
	{

	private:
		std::unique_ptr<DragonWindow::Window> m_pWindow;
		std::unique_ptr<DragonCore::ECS::Registry> m_pRegistry;

		SDL_Event m_Event;
		bool m_bIsRunning = false;

		// create the vertex shader array object and the vertex buffer object //Temp Test
		GLuint VAO, VBO, IBO; //Temp Test

		bool Initialize();
		bool LoadShaders();

		void ProcessEvents();
		void Update();
		void Render();

		void CleanUp();

		Application();

	public:
		static Application& GetInstance();
		~Application();

		void Run();
	};
}