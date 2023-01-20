#pragma once
#include "RendererAPI.h"

namespace Explore
{
	//static swapper class which contains RendererAPI
	//its level is beneath to Renderer class
	class RenderCommand
	{
	public:
		static inline void Init()
		{
			m_RendererAPI->Init();
		}

		static inline void SetViewport(uint32_t xOffset,uint32_t yOffset,uint32_t width,uint32_t height)
		{
			m_RendererAPI->SetViewport(xOffset, yOffset, width, height);
		}

		static inline void SetClearColor(const glm::vec4& color)
		{
			m_RendererAPI->SetClearColor(color);
		}

		static inline void Clear()
		{
			m_RendererAPI->Clear();
		}

		static inline void DrawIndexd(const Ref<VertexArray>& vertexArray)
		{
			m_RendererAPI->DrawIndexd(vertexArray);
		}

	private:
		static Ref<RendererAPI> m_RendererAPI;
	};
}