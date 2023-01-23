#pragma once
#include "OrthographicCamera.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

namespace Explore
{
	//static interface for 2D Renderer
	class Renderer2D
	{
	public:
		static void Init();

		static void Shutdown();

		static void BeginScene(OrthographicCamera& camera);

		static void EndScene();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture);

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture);

	public:
		struct Renderer2DStorage
		{
			Ref<VertexArray>	QuadVertexArray;
			Ref<Shader>			TextureShader;
			Ref<Texture>		WhiteTexture;
		};

	private:
		static Renderer2DStorage* s_Renderer2DData;
	};
}
