#pragma once
#include "RenderCommand.h"
#include "Shader.h"
#include "OrthographicCamera.h"

namespace Explore
{
	//Provide Rendering Functions for Renderer
	//its level is high to RenderCommand
	class Renderer
	{
	public:
		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		static void BeginScene(OrthographicCamera& camera);

		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4 transform);
	
		struct SceneData
		{
			glm::mat4 viewProjectionMatrix;
		};

	private:
		static SceneData s_SceneData;
	};
}