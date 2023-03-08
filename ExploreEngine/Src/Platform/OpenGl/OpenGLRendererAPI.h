#pragma once
#include "Explore/Renderer/RendererAPI.h"

namespace Explore
{
	//RendererAPI which is implemented by OpenGL
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;

		virtual void SetViewport(uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height) override;

		virtual void SetClearColor(const glm::vec4& color) override;

		virtual void Clear() override;

		virtual void DrawIndexd(const Ref<VertexArray>& vertexArray, uint32_t indexCount) override;
	};
}