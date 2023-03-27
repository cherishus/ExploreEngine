#include "enginepch.h"
#include "SubTexture.h"

namespace Explore
{
	SubTexture::SubTexture(Ref<Texture> texture, glm::vec2 min, glm::vec2 max)
		: m_Texture(texture)
	{
		m_Coods.resize(4);
		m_Coods[0] = { min.x, min.y };
		m_Coods[1] = { max.x, min.y };
		m_Coods[2] = { max.x, max.y };
		m_Coods[3] = { min.x, max.y };
	}

	Ref<SubTexture> SubTexture::CrereFromTexture(Ref<Texture> texture, glm::vec2 cellSize, glm::vec2 coods, glm::vec2 spriteSize)
	{
		glm::vec2 min = { (coods.x * cellSize.x) / texture->GetWidth(), coods.y * cellSize.y / texture->GetHeight() };
		glm::vec2 max = { (coods.x + spriteSize.x) * cellSize.x / texture->GetWidth(), (coods.y + spriteSize.y) * cellSize.y / texture->GetHeight() };
		return std::make_shared<SubTexture>(texture, min, max);
	}
}