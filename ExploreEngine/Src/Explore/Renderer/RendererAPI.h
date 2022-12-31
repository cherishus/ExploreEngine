#pragma once
#include "VertexArray.h"
#include "glm/vec4.hpp"

namespace Explore
{
	//Abstract API without plantform for Renderer or RenderCommand
	class RendererAPI
	{
	public:
		//Determine to use which plantform
		enum class API : uint8_t
		{
			None = 0,
			OpenGL = 1
		};

		virtual void SetClearColor(const glm::vec4& color) = 0;

		virtual void Clear() = 0;

		virtual void DrawIndexd(const std::shared_ptr<VertexArray>& vertexArray) = 0;

		static RendererAPI* Create();

		static API GetAPI() { return s_API; }
	
	private:
		static API s_API;
	};
}