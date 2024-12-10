#pragma once

#include <glad/glad.h>
#include <string>

namespace DragonRendering
{
	enum class TextureType
	{
		PIXEL = 0, BLENDED, NONE
	};

	class Texture
	{
	private:

		GLuint m_TextureID;

		int m_Width, m_Height;

		std::string m_sPath;

		TextureType m_eType;
	public: 
		Texture();
		Texture(GLuint ID, int Width, int Height, TextureType Type = TextureType::PIXEL, const std::string& TexturePath = "");
		~Texture() = default;

		inline const int GetWidth() const { return m_Width; }
		inline const int GetHeight() const { return m_Height; }
		inline const GLuint GetID() const { return m_TextureID; }
		inline const std::string& GetPath() const { return m_sPath; }
		inline const TextureType GetType() const { return m_eType; }

		void Bind();
		void Unbind();
	};
}
