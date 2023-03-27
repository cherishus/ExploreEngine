#pragma once
#include "Texture.h"
#include "glm/glm.hpp"

namespace Explore
{
	class SubTexture
	{
	public:
		SubTexture(Ref<Texture> texture, glm::vec2 min, glm::vec2 max);

		Ref<Texture> GetTexture() { return m_Texture; }

		std::vector<glm::vec2> GetCoods() { return m_Coods; }

		static Ref<SubTexture> CrereFromTexture(Ref<Texture> texture, glm::vec2 cellSize, glm::vec2 coods, glm::vec2 spriteSize);

	private:
		Ref<Texture> m_Texture;

		std::vector<glm::vec2> m_Coods;
	};
}