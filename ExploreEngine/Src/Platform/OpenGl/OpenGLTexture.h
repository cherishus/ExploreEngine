#pragma once
#include "Explore/Renderer/Texture.h"

namespace Explore
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);

		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() override { return m_Width; }

		virtual uint32_t GetHeight() override { return m_Height; }

		virtual void Bind(uint32_t slot = 0) override;

	private:
		std::string m_Path;

		uint32_t m_Width;

		uint32_t m_Height;

		uint32_t m_TextureID; //TextureId in GPU
	};
}