#include "Texture.h"

namespace DragonRendering
{
	Texture::Texture() : Texture(0,0,0,TextureType::NONE)
	{

	}

	Texture::Texture(GLuint ID, int Width, int Height, TextureType Type, const std::string& TexturePath) :
		m_TextureID{ ID }, m_Width{ Width }, m_Height{ Height }, m_eType{ Type }, m_sPath{ TexturePath }
	{

	}

	void Texture::Bind()
	{
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
	}

	void Texture::Unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
