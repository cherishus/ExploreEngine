#pragma once
#include "Explore/Renderer/RendererAPI.h"

namespace Explore
{
	//RendererAPI which is implemented by OpenGL
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColor(const glm::vec4& color) override;

		virtual void Clear() override;

		virtual void DrawIndexd(const std::shared_ptr<VertexArray>& vertexArray) override;
	};
}