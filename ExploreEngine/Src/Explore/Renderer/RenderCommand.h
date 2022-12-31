#pragma once
#include "RendererAPI.h"

namespace Explore
{
	//static swapper class which contains RendererAPI
	//its level is beneath to Renderer class
	class RenderCommand
	{
	public:
		static inline void SetClearColor(const glm::vec4& color)
		{
			m_RendererAPI->SetClearColor(color);
		}

		static inline void Clear()
		{
			m_RendererAPI->Clear();
		}

		static inline void DrawIndexd(const std::shared_ptr<VertexArray>& vertexArray)
		{
			m_RendererAPI->DrawIndexd(vertexArray);
		}

	private:
		static std::shared_ptr<RendererAPI> m_RendererAPI;
	};
}