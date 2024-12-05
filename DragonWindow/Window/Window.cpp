#include "Window.h"
#include <iostream>

void DragonWindow::Window::CreateNewWindow(Uint32 Flags)
{
	m_pWindow = WindowPtr(SDL_CreateWindow(m_sTitle.c_str(), m_XPosition, m_YPosition, m_Width, m_Height, m_WindowFlags));

	if (!m_pWindow)
	{
		std::string error = SDL_GetError();
		std::cout << "Failed to create the window " << error << "\n";
	}
}

DragonWindow::Window::Window(const std::string title, int width, int height, int xPosition, int yPosition, bool bVSync, Uint32 flags) : m_pWindow{nullptr}, m_GLContext{}, m_sTitle{title}, m_Width{width}, m_Height{height}, m_XPosition{xPosition}, m_YPosition{yPosition}, m_WindowFlags{flags}
{
	CreateNewWindow(flags);

	//Enable VSync
	if (bVSync)
	{
		if (!SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1"))
		{
			std::cout << "Failed to enable VSync!\n";
		}
	}

	std::cout << "Window created Successfuly!\n";
}

void DragonWindow::Window::SetWindowName(const std::string& name)
{
	m_sTitle = name;
	SDL_SetWindowTitle(m_pWindow.get(), name.c_str());
}

DragonWindow::Window::~Window()
{
}
