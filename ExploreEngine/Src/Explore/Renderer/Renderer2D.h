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
		struct QuadVertex
		{
			glm::vec3 Position;
			glm::vec4 Color;
			glm::vec2 TexCoord;
			float TexIndex;
			float TilingFactor;
		};

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t DrawQuads = 0;

			uint32_t GetTotalVertexCounts() { return DrawQuads * 4; }
			uint32_t GetTotalIndexCounts() { return DrawQuads * 6; }
		};

		struct Renderer2DData
		{
			const uint32_t MaxQuads = 10000;
			const uint32_t MaxVertices = MaxQuads * 4;
			const uint32_t MaxIndices = MaxQuads * 6;
			static const uint32_t MaxTextureSlots = 32; //direct initialization in stuct for static const member

			Ref<VertexArray>	QuadVertexArray;
			Ref<VertexBuffer>	QuadVertexBuffer;
			Ref<Shader>			TextureShader;
			Ref<Texture>		WhiteTexture;

			uint32_t QuadIndexCount = 0;
			QuadVertex* QuadVertexBufferBase = nullptr;
			QuadVertex* QuadVertexBufferPtr = nullptr;
			glm::vec4 QuadPosition[4];

			/*
			@breif: std::array is a container with fixed size
			*/
			std::array<Ref<Texture>, MaxTextureSlots> Textures;
			uint32_t TextureSlotIndex = 1; //0:white texture

			Statistics Stats;
		};

	public:
		static void Init();

		static void Shutdown();

		static void BeginScene(OrthographicCamera& camera);

		static void EndScene();

		static void Flush();

		static void FlushAndReset();

		static void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color);

		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color);

		static void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor = 1.0f);

		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture>& texture,float tilingFactor = 1.0f);

		static void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color, const Ref<Texture>& texture, float tilingFactor = 1.0f);

		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color, const Ref<Texture>& texture,float tilingFactor = 1.0f);

		static void ResetStats();

		static Statistics GetStats();

	private:
		static Renderer2DData s_Renderer2DData;
	};
}
