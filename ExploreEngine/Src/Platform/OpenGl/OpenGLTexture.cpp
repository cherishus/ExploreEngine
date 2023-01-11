#include "enginepch.h"
#include "OpenGLTexture.h"
#include "Explore/Log.h"
#include "glad/glad.h"
#include "stb_image.h"

namespace Explore
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
	{
		int width, height, channel;
		stbi_set_flip_vertically_on_load(1); //flip vertically
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channel, 0);
		EXPLORE_ASSERT(data, "load texture file failed!");
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channel == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		else if (channel == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		EXPLORE_ASSERT(internalFormat & dataFormat, "texture format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
		glTextureStorage2D(m_TextureID, 1, internalFormat, m_Width, m_Height); //levers = 1, none mipmap

		//set texture parameter about Texture Filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_TextureID);
	}
	
	void OpenGLTexture2D::Bind(uint32_t slot)
	{
		//slotIndex is meaning texture unit
		glBindTextureUnit(slot, m_TextureID);
	}
}