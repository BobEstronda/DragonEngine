#define SDL_MAIN_HANDLED 1;

#include <Window/Window.h>
#include <glad/glad.h>
#include <iostream>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Rendering/Essentials/ShaderLoader.h>
#include <Rendering/Essentials/TextureLoader.h>
#include <Rendering/Core/Camera2D.h>

#include "Logger/Logger.h"

struct UVs
{
	float u, v, width, height;

	UVs() : u{ 0.f }, v{ 0.f }, width{ 0.f }, height{ 0.f }
	{

	}
};

int main() 
{
	DRAGON_INIT_LOGS(true, true);
	bool bRunning = true;

	// init SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::string error = SDL_GetError();
		std::cout << "Failed to initialize SDL!" << error << std::endl;

		bRunning = false;
		return -1;
	}

	// Set Up OpenGL
	if (SDL_GL_LoadLibrary(NULL) != 0)
	{
		std::string error = SDL_GetError();
		std::cout << "Failed to openGl Library!" << error << std::endl;

		bRunning = false;
		return -1;
	}

	// Set Open GL Attributes 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//Set The Number Of bits per channel
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// Create Window 
	DragonWindow::Window Window("Test DragonWindow", 640, 480, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, true, SDL_WINDOW_OPENGL);

	if (!Window.GetWindow())
	{
		std::cout << "Failed the create the window" << std::endl;

		bRunning = false;
		return -1;
	}

	//Create the Open Gl Context
	Window.SetGLContext(SDL_GL_CreateContext(Window.GetWindow().get()));
	if (!Window.GetGLContext())
	{
		std::string error = SDL_GetError();
		std::cout << "Failed to create Open GL Context!" << error << std::endl;

		bRunning = false;
		return -1;
	}

	SDL_GL_MakeCurrent(Window.GetWindow().get(), Window.GetGLContext());
	SDL_GL_SetSwapInterval(1);

	//Initialize Glad
	if (gladLoadGLLoader(SDL_GL_GetProcAddress) == 0)
	{
		std::cout << "Failed to LoadGL -> Glad" << std::endl;

		bRunning = false;
		return -1;
	}

	//Enable Alpha Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	auto Texture = DragonRendering::TextureLoader::Create(DragonRendering::TextureType::PIXEL, 
		"assets/textures/Checkpoint (Flag Idle)(64x64).png");

	if (!Texture)
	{
		DRAGON_ERROR("Failed to create the Texture!");
		return -1;
	}

	// make UVs
	UVs uvs{};
	auto generateUVs = [&](float startX, float startY, float spriteWidth, float spriteHeight)
	{
		uvs.width = spriteWidth / Texture->GetWidth();
		uvs.height = spriteHeight / Texture->GetHeight();

		uvs.u = startX * uvs.width;
		uvs.v = startY * uvs.height;
	};

	generateUVs(2, 0, 64, 64);

	//create temp vertex data
	float vertices[] =
	{
		10.f, 26.f, 0.0f, uvs.u, (uvs.v + uvs.height),              //TL 
		10.f, 10.f, 0.0f, uvs.u, uvs.v,                             //BL
		26.f, 10.f, 0.0f, (uvs.u + uvs.width), uvs.v,               //BR
		26.f, 26.f, 0.0f, (uvs.u + uvs.width), (uvs.v + uvs.height) //TR
	};

	GLuint indices[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	//create Camera
	DragonRendering::Camera2D camera{}; 
	camera.SetScale(5.f);

	// Create out first shader
	auto Shader = DragonRendering::ShaderLoader::Create("assets/shaders/BasicShader.vert", "assets/shaders/BasicShader.frag");

	if (!Shader)
	{
		std::cout << "Failed to create the shader!" << std::endl;
		return -1;
	}
	
	// create the vertex shader array object and the vertex buffer object
	GLuint VAO, VBO, IBO;

	// generate the VAO
	glGenVertexArrays(1, &VAO);

	// generate the VBO 
	glGenBuffers(1, &VBO);

	// bind VAO and IBO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(
		GL_ARRAY_BUFFER,                      //the target buffer type
		sizeof(vertices),                     //the size in bytes of the buffer object's new data store
		vertices,                             //A pointer to the data that will be copied into the data store
		GL_STATIC_DRAW                        //the expectedusage pattern of the data store
	);

	// bind IBO
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,              //the target buffer type
		6 * sizeof(GLuint),                   //the size in bytes of the buffer object's new data store
		indices,                              //A pointer to the data that will be copied into the data store
		GL_STATIC_DRAW                        //the expectedusage pattern of the data store
	);

	glVertexAttribPointer(
		0,                                    //attribute 0 == the layout position in the shader
		3,                                    //Size == Number of components per vertex
		GL_FLOAT,                             //Type == the data type of the above components
		GL_FALSE,                             //Normalized == Specifies if fixed-point data values should be normalized
		5 * sizeof(float),                    //Stride == Specifies the byte offset between consecutive attributes
		(void*)0                              //Pointer == Specifies the offset of the first component 
	);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		5 * sizeof(float),
		reinterpret_cast<void*>(sizeof(float) * 3)  //this the offset of the position data to the first UV coordinate
	);

	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	SDL_Event event{};

	//Window loop
	while (bRunning)
	{
		//process event
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				bRunning = false;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					bRunning = false;
					break;
				}
			default:
				break;
			}
		}

		glViewport(0, 0, Window.GetWidth(), Window.GetHeight());

		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		Shader->Enable();

		glBindVertexArray(VAO);

		auto projection = camera.GetCameraMatrix();

		Shader->SetUniformMat4("uProjection", projection);

		glActiveTexture(GL_TEXTURE);
		glBindTexture(GL_TEXTURE_2D, Texture->GetID());

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(0);

		SDL_GL_SwapWindow(Window.GetWindow().get());

		camera.Update();
		Shader->Disable();
	}

	std::cout << "Closing Editor!" << std::endl;
	return 0;
}