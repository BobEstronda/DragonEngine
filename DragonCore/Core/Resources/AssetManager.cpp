#include "AssetManager.h"

#include <Logger/Logger.h>
#include <Rendering/Essentials/TextureLoader.h>
#include <Rendering/Essentials/ShaderLoader.h>


namespace DragonCore::Resources
{
    const bool AssetManager::AddTexture(const std::string& TextureName, const std::string& TexturePath, const bool Pixel)
    {
        //check to the texture already exist
        if (m_mapTexture.find(TextureName) != m_mapTexture.end())
        {
            DRAGON_ERROR("Failed to add texture [{0}], the texture already exist!", TextureName);
            return false;
        }

        const DragonRendering::TextureType TextureType = Pixel ? DragonRendering::TextureType::PIXEL : DragonRendering::TextureType::BLENDED;
        auto Texture = std::move(DragonRendering::TextureLoader::Create(TextureType, TexturePath));

        if (!Texture)
        {
            DRAGON_ERROR("Failed to load texture [{0}], at path [{1}]", TextureName, TexturePath);
            return false;
        }

        m_mapTexture.emplace(TextureName, std::move(Texture));
        return true;
    }

    const DragonRendering::Texture& AssetManager::GetTexture(const std::string& TextureName)
    {
        auto TextItr = m_mapTexture.find(TextureName);

        if (TextItr == m_mapTexture.end())
        {
            DRAGON_ERROR("Failed to get texture [{0}], the texture does not exist!", TextureName);
            return DragonRendering::Texture();
        }

        return *TextItr->second;
    }

    const bool AssetManager::AddShader(const std::string& ShaderName, const std::string& VertexPath, const std::string& FragmentPath)
    {
        //check to the shader already exist
        if (m_mapShader.find(ShaderName) != m_mapShader.end())
        {
            DRAGON_ERROR("Failed to add shader [{0}], the shader already exist!", ShaderName);
            return false;
        }

        auto Shader = std::move(DragonRendering::ShaderLoader::Create(VertexPath, FragmentPath));

        if (!Shader)
        {
            DRAGON_ERROR("Failed to load Shader [{0}], at vertex path [{1}] and fragment path [{2}]", ShaderName, VertexPath, FragmentPath);
            return false;
        }

        m_mapShader.emplace(ShaderName, std::move(Shader));
        return true;
    }

    DragonRendering::Shader& AssetManager::GetShader(const std::string& ShaderName)
    {
        auto ShaderItr = m_mapShader.find(ShaderName);

        if (ShaderItr == m_mapShader.end())
        {
            DRAGON_ERROR("Failed to get Shader [{0}], the texture does not exist!", ShaderName);
            DragonRendering::Shader Shader{};
            return Shader;
        }

        return *ShaderItr->second;
    }
}
