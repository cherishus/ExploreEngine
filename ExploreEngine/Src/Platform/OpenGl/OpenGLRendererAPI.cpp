#include "enginepch.h"
#include "OpenGLRendererAPI.h"
#include "glad/glad.h"

namespace Explore
{
	void OpenGLRendererAPI::Init()
	{
		EXPLORE_PROFILE_FUNCTION()
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height)
	{
		EXPLORE_PROFILE_FUNCTION()
		glViewport(xOffset, yOffset, width, height); //tell openGL the size of render area
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		EXPLORE_PROFILE_FUNCTION()
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		EXPLORE_PROFILE_FUNCTION()
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
	void OpenGLRendererAPI::DrawIndexd(const Ref<VertexArray>& vertexArray)
	{
		EXPLORE_PROFILE_FUNCTION()
		vertexArray->Bind();
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);
	}
}