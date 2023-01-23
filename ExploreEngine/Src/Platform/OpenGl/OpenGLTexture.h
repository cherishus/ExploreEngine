#pragma once
#include "Explore/Renderer/Texture.h"
#include "glad/glad.h"

namespace Explore
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);

		OpenGLTexture2D(uint32_t width, uint32_t height); //create non data texture

		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() override { return m_Width; }

		virtual uint32_t GetHeight() override { return m_Height; }

		virtual void Bind(uint32_t slot = 0) override;

		virtual void SetData(void* data, uint32_t size) override;

	private:
		std::string m_Path;

		uint32_t m_Width;

		uint32_t m_Height;

		GLenum m_InternalFormat, m_DataFormat;

		uint32_t m_TextureID; //TextureId in GPU
	};
}