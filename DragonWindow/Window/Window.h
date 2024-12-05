#pragma once

#include <DragonUtilities/SDL_Wrappers.h>
#include <string>

namespace DragonWindow
{
	class Window
	{

	private:
		WindowPtr m_pWindow;
		SDL_GLContext m_GLContext;
		std::string m_sTitle;
		int m_Width, m_Height, m_XPosition, m_YPosition;
		Uint32 m_WindowFlags;

		void CreateNewWindow(Uint32 Flags);
	public:

		Window() : Window("default", 640, 480, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, true, NULL)
		{

		}

		Window(const std::string title, int width, int height, int xPosition, int yPosition, bool bVSync = true,
			Uint32 flags = (SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MOUSE_CAPTURE));

		inline void SetGLContext(SDL_GLContext gl_context) { m_GLContext = gl_context; }
		inline SDL_GLContext& GetGLContext() { return m_GLContext; }

		inline WindowPtr& GetWindow() { return m_pWindow; }

		inline const std::string& GetWindowName() { return m_sTitle; }
		void SetWindowName(const std::string& name);

		inline void SetWidth(const int width) { m_Width = width; }
		inline int GetWidth() const { return m_Width; }
		inline void SetHeight(const int height) { m_Height = height; }
		inline int GetHeight() const { return m_Height; }

		inline void SetXPosition(const int xPosition) { m_XPosition = xPosition; }
		inline int GetXPosition() const { return m_XPosition; }
		inline void SetYPosition(const int yPosition) { m_YPosition = yPosition; }
		inline int GetYPosition() const { return m_YPosition; }

		~Window();

	};
}