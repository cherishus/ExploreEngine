#pragma once
#include "Explore/Core/Core.h"

namespace Explore {
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() = 0;

		virtual uint32_t GetHeight() = 0;
		
		virtual void Bind(uint32_t slot = 0) = 0;

		virtual void SetData(void* data, uint32_t size) = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture> Create(const std::string& path);

		static Ref<Texture> Create(uint32_t width, uint32_t height);
	};
}