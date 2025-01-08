#include "Application.h"

#include <iostream>
#include "Logger/Logger.h"
//#include <glad/glad.h>
//#include <Window/Window.h>

#include <Rendering/Essentials/ShaderLoader.h>
#include <Rendering/Essentials/TextureLoader.h>
#include <Rendering/Essentials/Vertex.h>
#include <Rendering/Core/Camera2D.h>

#include <Core/ECS/Entity.h>
#include <Core/ECS/Components/IdentificationComponent.h>
#include <Core/ECS/Components/TransformComponent.h>
#include <Core/ECS/Components/SpriteComponent.h>

#include <Core/Resources/AssetManager.h>

namespace DragonEditor
{

	bool Application::Initialize()
	{
		DRAGON_INIT_LOGS(true, true);

		//bool bRunning = true;

		// init SDL
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			std::string error = SDL_GetError();
			std::cout << "Failed to initialize SDL!" << error << std::endl;

			//bRunning = false;
			return false;
		}

		// Set Up OpenGL
		if (SDL_GL_LoadLibrary(NULL) != 0)
		{
			std::string error = SDL_GetError();
			std::cout << "Failed to openGl Library!" << error << std::endl;

			//bRunning = false;
			return false;
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
		/*DragonWindow::Window Window("Test DragonWindow", 640, 480, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, true, SDL_WINDOW_OPENGL);*/

		m_pWindow = std::make_unique<DragonWindow::Window>(
			"Test DragonWindow", 
			640, 480, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
			true, SDL_WINDOW_OPENGL);

		if (!m_pWindow->GetWindow())
		{
			std::cout << "Failed the create the window" << std::endl;

			//bRunning = false;
			return false;
		}

		//Create the Open Gl Context
		m_pWindow->SetGLContext(SDL_GL_CreateContext(m_pWindow->GetWindow().get()));
		if (!m_pWindow->GetGLContext())
		{
			std::string error = SDL_GetError();
			std::cout << "Failed to create Open GL Context!" << error << std::endl;

			//bRunning = false;
			return false;
		}

		SDL_GL_MakeCurrent(m_pWindow->GetWindow().get(), m_pWindow->GetGLContext());
		SDL_GL_SetSwapInterval(1);

		//Initialize Glad
		if (gladLoadGLLoader(SDL_GL_GetProcAddress) == 0)
		{
			std::cout << "Failed to LoadGL -> Glad" << std::endl;

			//bRunning = false;
			return false;
		}

		//Enable Alpha Blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Registry
		//auto pRegistry = std::make_unique<entt::registry>();

		auto AssetManager = std::make_shared<DragonCore::Resources::AssetManager>();
		if (!AssetManager)
		{
			DRAGON_ERROR("Failed to create the asset manager!");
			return false;
		}

		if (!AssetManager->AddTexture("FlagCheckpoint", "assets/textures/Checkpoint (Flag Idle)(64x64).png"))
		{
			DRAGON_ERROR("Failed to create the texture!");
			return false;
		}

		/*auto Texture = DragonRendering::TextureLoader::Create(DragonRendering::TextureType::PIXEL,
			"assets/textures/Checkpoint (Flag Idle)(64x64).png");*/

		auto& Texture = AssetManager->GetTexture("FlagCheckpoint");

		/*if (!Texture)
		{
			DRAGON_ERROR("Failed to create the Texture!");
			return -1;
		}*/


		// create new entity
		//auto ent1 = pRegistry->create();

		m_pRegistry = std::make_unique<DragonCore::ECS::Registry>();
		if (!m_pRegistry)
		{
			DRAGON_ERROR("Failed to create the entt registry!");
			return -1;
		}

		DragonCore::ECS::Entity entity1{ *m_pRegistry, "Ent1", "Test" };

		auto& transform = entity1.AddComponent<DragonCore::ECS::TransformComponent>(DragonCore::ECS::TransformComponent{
			.position = glm::vec2{ 10.f, 10.f } ,
			.scale = glm::vec2{1.f,1.f},
			.rotation = 0.f }
			);

		auto& sprite = entity1.AddComponent<DragonCore::ECS::SpriteComponent>(DragonCore::ECS::SpriteComponent{
			.width = 64,
			.height = 64,
			.color = DragonRendering::Color{.R = 0, .G = 255,.B = 0, .A = 255},
			.start_x = 2,
			.start_y = 0 }
			);

		sprite.GenerateUVs(Texture.GetWidth(), Texture.GetHeight());

		/*UVs uVs{};
		auto generateUVs = [&](float startX, float startY, float spriteWidth, float spriteHeight)
		{
			uVs.width = spriteWidth / Texture->GetWidth();
			uVs.height = spriteHeight / Texture->GetHeight();

			uVs.u = startX * uVs.width;
			uVs.v = startY * uVs.height;
		};

		generateUVs(2, 0, 64, 64);*/

		//create temp vertex data
		//float vertices[] =
		//{
		//	10.f, 26.f, 0.0f, uvs.u, (uvs.v + uvs.height),              //TL 
		//	10.f, 10.f, 0.0f, uvs.u, uvs.v,                             //BL
		//	26.f, 10.f, 0.0f, (uvs.u + uvs.width), uvs.v,               //BR
		//	26.f, 26.f, 0.0f, (uvs.u + uvs.width), (uvs.v + uvs.height) //TR
		//};

		std::vector<DragonRendering::Vertex> vertices{};
		DragonRendering::Vertex vTL{}, vTR{}, vBL{}, vBR{};

		vTL.position = glm::vec2{ /*10.f*/ transform.position.x, /*26.f*/  transform.position.y + sprite.height };
		vTL.uvs = glm::vec2{ /*uVs.u*/ sprite.uvs.u, /*(uVs.v + uVs.height)*/ sprite.uvs.v + sprite.uvs.uv_height };

		vTR.position = glm::vec2{ /*10.f*/ transform.position.x + sprite.width, /*10.f*/ transform.position.y + sprite.height };
		vTR.uvs = glm::vec2{ /*uVs.u*/  sprite.uvs.u + sprite.uvs.uv_width, /*uVs.v*/ sprite.uvs.v + sprite.uvs.uv_height };

		vBL.position = glm::vec2{ /*26.f*/ transform.position.x,/*10.f*/ transform.position.y };
		vBL.uvs = glm::vec2{ /*(uVs.u + uVs.width)*/  sprite.uvs.u , /*uVs.v*/  sprite.uvs.v };

		vBR.position = glm::vec2{ /*26.f*/ transform.position.x + sprite.width, /*26.f*/ transform.position.y };
		vBR.uvs = glm::vec2{ /*(uVs.u + uVs.width) */  sprite.uvs.u + sprite.uvs.uv_width , /*(uVs.v + uVs.height)*/ sprite.uvs.v };

		vertices.push_back(vTL);
		vertices.push_back(vBL);
		vertices.push_back(vBR);
		vertices.push_back(vTR);

		GLuint indices[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		/*auto& id = entity1.GetComponent<DragonCore::ECS::IdentificationComponent>();
		DRAGON_LOG("Name: {0}, Group: {1}, ID: {2}", id.name, id.group, id.entityID);*/

		//create Camera
		auto camera = std::make_shared<DragonRendering::Camera2D>();
		camera->SetScale(5.f);

		if (!m_pRegistry->AddToContext<std::shared_ptr<DragonCore::Resources::AssetManager>>(AssetManager))
		{
			DRAGON_ERROR("Failed to add asset manager to the registry context!");
			return false;
		}

		if (!m_pRegistry->AddToContext<std::shared_ptr<DragonRendering::Camera2D>>(camera))
		{
			DRAGON_ERROR("Failed to add camera to the registry context!");
			return false;
		}

		if (!LoadShaders())
		{
			DRAGON_ERROR("Failed to the Load Shaders");
			return false;
		}
		// Create out first shader
		//auto Shader = DragonRendering::ShaderLoader::Create("assets/shaders/BasicShader.vert", "assets/shaders/BasicShader.frag");

		/*if (!AssetManager->AddShader("BasicShader", "assets/shaders/BasicShader.vert", "assets/shaders/BasicShader.frag"))
		{
			DRAGON_ERROR("Failed to add Shader to the asset manager!");
			return false;
		}

		auto Shader = AssetManager->GetShader("BasicShader");

		if (Shader.GetShaderProgramID() == 0)
		{
			DRAGON_ERROR("Failed to create the shader!");
			return false;
		}*/

		// create the vertex shader array object and the vertex buffer object
		//GLuint VAO, VBO, IBO;

		// generate the VAO
		glGenVertexArrays(1, &VAO);

		// generate the VBO 
		glGenBuffers(1, &VBO);

		// bind VAO and IBO
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(
			GL_ARRAY_BUFFER,                                         //the target buffer type
			vertices.size() * sizeof(DragonRendering::Vertex),       //the size in bytes of the buffer object's new data store
			vertices.data(),                                         //A pointer to the data that will be copied into the data store
			GL_STATIC_DRAW                                           //the expectedusage pattern of the data store
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
			0,                                                 //attribute 0 == the layout position in the shader
			2,                                                 //Size == Number of components per vertex
			GL_FLOAT,                                          //Type == the data type of the above components
			GL_FALSE,                                          //Normalized == Specifies if fixed-point data values should be normalized
			sizeof(DragonRendering::Vertex),                   //Stride == Specifies the byte offset between consecutive attributes
			(void*)offsetof(DragonRendering::Vertex, position) //Pointer == Specifies the offset of the first component 
		);

		glEnableVertexAttribArray(0);

		glVertexAttribPointer(
			1,
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof(DragonRendering::Vertex),
			(void*)offsetof(DragonRendering::Vertex, uvs)  //this the offset of the position data to the first UV coordinate
		);

		glEnableVertexAttribArray(1);

		glVertexAttribPointer(
			2,
			4,
			GL_UNSIGNED_BYTE,
			GL_TRUE,                                       // [0,1]  
			sizeof(DragonRendering::Vertex),
			(void*)offsetof(DragonRendering::Vertex, color)  //this the offset of the position data to the first UV coordinate
		);

		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
	}

	bool Application::LoadShaders()
	{
		auto& AssetManager = m_pRegistry->GetContext<std::shared_ptr<DragonCore::Resources::AssetManager>>();

		if (!AssetManager)
		{
			DRAGON_ERROR("Failed to get asset manager from the registry context");
			return false;
		}

		if (!AssetManager->AddShader("BasicShader", "assets/shaders/BasicShader.vert", "assets/shaders/BasicShader.frag"))
		{
			DRAGON_ERROR("Failed to add Shader to the asset manager!");
			return false;
		}

		return true;

		/*auto Shader = AssetManager->GetShader("BasicShader");

		if (Shader.GetShaderProgramID() == 0)
		{
			DRAGON_ERROR("Failed to create the shader!");
			return false;
		}*/
	}

	void Application::ProcessEvents()
	{
		//process event
		while (SDL_PollEvent(&m_Event))
		{
			switch (m_Event.type)
			{
			case SDL_QUIT:
				m_bIsRunning = false;
				break;
			case SDL_KEYDOWN:
				if (m_Event.key.keysym.sym == SDLK_ESCAPE)
				{
					m_bIsRunning = false;
					break;
				}
			default:
				break;
			}
		}
	}

	void Application::Update()
	{
		auto& camera = m_pRegistry->GetContext<std::shared_ptr<DragonRendering::Camera2D>>();
		if (!camera)
		{
			DRAGON_ERROR("Failed to get camera from the registry context");
			return;
		}

		camera->Update();
	}

	void Application::Render()
	{
		auto& AssetManager = m_pRegistry->GetContext<std::shared_ptr<DragonCore::Resources::AssetManager>>();
		auto& camera = m_pRegistry->GetContext<std::shared_ptr<DragonRendering::Camera2D>>();

		auto& Shader = AssetManager->GetShader("BasicShader");
		auto Projection = camera->GetCameraMatrix();

		if (Shader.GetShaderProgramID() == 0)
		{
			DRAGON_ERROR("Shader program has not be create correctly");
			return;
		}

		glViewport(0, 0, m_pWindow->GetWidth(), m_pWindow->GetHeight());

		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		Shader.Enable();

		glBindVertexArray(VAO);

		Shader.SetUniformMat4("uProjection", Projection);

		glActiveTexture(GL_TEXTURE);
		const auto& Texture = AssetManager->GetTexture("FlagCheckpoint");
		glBindTexture(GL_TEXTURE_2D, Texture.GetID());

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(0);

		SDL_GL_SwapWindow(m_pWindow->GetWindow().get());

		//camera.Update();
		Shader.Disable();
	}

	void DragonEditor::Application::CleanUp()
	{
		SDL_Quit();
	}

	Application::Application() : m_pWindow{nullptr}, m_pRegistry{nullptr}, m_bIsRunning{true},
		VAO{ 0 }, VBO{ 0 }, IBO{ 0 } //temporary test, remove later
	{
	}

	Application& Application::GetInstance()
	{
		static Application App{};
		return App;
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		if (!Initialize())
		{
			DRAGON_ERROR("Initialization Failed!");
			return;
		}

		while (m_bIsRunning)
		{
			ProcessEvents();
			Update();
			Render();
		}

		CleanUp();
	}

}