#pragma once

namespace Explore {
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() = 0;

		virtual uint32_t GetHeight() = 0;
		
		virtual void Bind(uint32_t slot = 0) = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Texture* Create(const std::string& path);
	};
}