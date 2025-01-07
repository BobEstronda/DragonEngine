#pragma once

#include <map>
#include <memory>
#include <string>

#include <Rendering/Essentials/Shader.h>
#include <Rendering/Essentials/Texture.h>

namespace DragonCore::Resources
{
	class AssetManager
	{
	private:
		std::map<std::string, std::shared_ptr<DragonRendering::Texture>> m_mapTexture{};
		std::map<std::string, std::shared_ptr<DragonRendering::Shader>> m_mapShader{};
	public:
		AssetManager() = default;
		~AssetManager() = default;

		const bool AddTexture(const std::string& TextureName, const std::string& TexturePath, const bool Pixel = true);
		const DragonRendering::Texture& GetTexture(const std::string& TextureName);

		const bool AddShader(const std::string& ShaderName, const std::string& VertexPath, const std::string& FragmentPath);
		const DragonRendering::Shader& GetShader(const std::string& ShaderName);
	};
}