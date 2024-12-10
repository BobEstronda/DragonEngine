#include "TextureLoader.h"

#include <SOIL/SOIL.h>
#include "Logger/Logger.h"

namespace DragonRendering 
{
    bool TextureLoader::LoadTexture(const std::string& FilePath, GLuint& ID, int& Width, int& Height, bool Blended)
    {
		int channels = 0;

		unsigned char* image = SOIL_load_image(
			FilePath.c_str(),                  //Filename == image file to be loaded
			&Width,                            //Width == Width of the image
			&Height,                           //height == height of the image
			&channels,                         //Channels == number of channels
			SOIL_LOAD_AUTO                     //ForceChannels == Force the channels count 
		);

		if (!image)
		{
			DRAGON_ERROR("SOIL Failed to load Image [{0}] -- {1}", FilePath, SOIL_last_result());
			return false;
		}

		GLint format = GL_RGBA;

		if (channels == 3)
		{
			format = GL_RGB;
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if (Blended)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}

		glTexImage2D(
			GL_TEXTURE_2D,                       //target == especifies the target texture
			0,                                   //level == level of detail. 0 is base image level
			format,                              //internal format == the number of color components
			Width,                               //width == width of the texture image
			Height,                              //height == height of the texture image
			0,                                   //border
			format,                              //format == format of the pixel data
			GL_UNSIGNED_BYTE,                    //type == the data type of the pixel data
			image                                //data
		);

		// Delete image Data from SOIL
		SOIL_free_image_data(image);

		return true;
    }

    std::shared_ptr<Texture> TextureLoader::Create(TextureType Type, const std::string& TexturePath)
    {
		GLuint ID;

		int Width, Height;

		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);

		bool bSuccesToCreateTexture = false;

		switch (Type)
		{
		case TextureType::PIXEL:
			bSuccesToCreateTexture = LoadTexture(TexturePath,ID, Width, Height);
			break;
		case TextureType::BLENDED:
			bSuccesToCreateTexture = LoadTexture(TexturePath, ID, Width, Height,true);
			break;
		default:

			DRAGON_WARN("The current texture type is not defined, please use a defined texture type!");
			return nullptr;
		}

		if (!bSuccesToCreateTexture)
		{
			DRAGON_ERROR("Failed to create Texture![{0}]", TexturePath);
		}

		return std::make_shared<Texture>(ID, Width, Height, Type,TexturePath);
    }
}
