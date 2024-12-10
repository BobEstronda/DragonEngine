#pragma once

#include "Texture.h"
#include <memory>

namespace DragonRendering
{
	class TextureLoader
	{
	private:

		static bool LoadTexture(const std::string& FilePath,GLuint& ID, int& Width, int& Height, bool Blended = false);

	public:
		TextureLoader() = delete;

		static std::shared_ptr<Texture> Create(TextureType Type, const std::string& TexturePath);
	};
}
