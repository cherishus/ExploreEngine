#include "enginepch.h"
#include "Renderer2D.h"
#include "RenderCommand.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Explore
{
	Renderer2D::Renderer2DData Renderer2D::s_Renderer2DData;

	void Renderer2D::Init()
	{
		EXPLORE_PROFILE_FUNCTION()

		//vertex array
		s_Renderer2DData.QuadVertexArray = VertexArray::Create();

		//vertex buffer
		s_Renderer2DData.QuadVertexBuffer = VertexBuffer::Create(s_Renderer2DData.MaxVertices * sizeof(QuadVertex));
		BufferLayout layout = {
			{"aPos",   ShaderDataType::Float3},
			{"aColor", ShaderDataType::Float4},
			{"aTexCoor",   ShaderDataType::Float2},
			{"aTexIndex", ShaderDataType::Float},
			{"aTilingFactor", ShaderDataType::Float}
		};
		s_Renderer2DData.QuadVertexBuffer->SetLayout(layout);
		s_Renderer2DData.QuadVertexArray->AddVertexBuffer(s_Renderer2DData.QuadVertexBuffer);

		//vertex data pointer
		s_Renderer2DData.QuadVertexBufferBase = new QuadVertex[s_Renderer2DData.MaxVertices];

		//index buffer
		uint32_t* quadIndices = new uint32_t[s_Renderer2DData.MaxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Renderer2DData.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(quadIndices, s_Renderer2DData.MaxIndices);
		s_Renderer2DData.QuadVertexArray->SetIndexBuffer(indexBuffer);
		delete[] quadIndices;
	
		//white texture
		s_Renderer2DData.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t data = 0xFFFFFFFF; //white texture: rgba = 1
		s_Renderer2DData.WhiteTexture->SetData(&data, sizeof(uint32_t));
		s_Renderer2DData.Textures[0] = s_Renderer2DData.WhiteTexture;

		int sampler[s_Renderer2DData.MaxTextureSlots];
		for (int i = 0; i < s_Renderer2DData.MaxTextureSlots; i++)
		{
			sampler[i] = i;
		}

		//shader
		s_Renderer2DData.TextureShader = Shader::Create("assets/shaders/texture.glsl");
		s_Renderer2DData.TextureShader->Bind(); //need to bind
		s_Renderer2DData.TextureShader->SetIntArray("u_Textures", sampler, s_Renderer2DData.MaxTextureSlots); //set sampler value

		//quad position
		s_Renderer2DData.QuadPosition[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Renderer2DData.QuadPosition[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Renderer2DData.QuadPosition[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Renderer2DData.QuadPosition[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}
	
	void Renderer2D::Shutdown()
	{
		EXPLORE_PROFILE_FUNCTION()
		
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		EXPLORE_PROFILE_FUNCTION()

		s_Renderer2DData.TextureShader->Bind();
		s_Renderer2DData.TextureShader->SetMat4("u_ProjectionViewMatrix", camera.GetViewProjectionMatrix());

		s_Renderer2DData.QuadIndexCount = 0;
		s_Renderer2DData.QuadVertexBufferPtr = s_Renderer2DData.QuadVertexBufferBase;
		s_Renderer2DData.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		EXPLORE_PROFILE_FUNCTION()

		//get vertex data and set to vertex buffer
		uint32_t dataSize = (uint8_t*)s_Renderer2DData.QuadVertexBufferPtr - (uint8_t*)s_Renderer2DData.QuadVertexBufferBase;
		s_Renderer2DData.QuadVertexBuffer->SetData(s_Renderer2DData.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		EXPLORE_PROFILE_FUNCTION()
		//Bind Texture
		for (uint32_t i = 0; i < s_Renderer2DData.TextureSlotIndex; i++)
		{
			s_Renderer2DData.Textures[i]->Bind(i);
		}
		RenderCommand::DrawIndexd(s_Renderer2DData.QuadVertexArray, s_Renderer2DData.QuadIndexCount);
		s_Renderer2DData.Stats.DrawCalls++;
	}

	void Renderer2D::FlushAndReset()
	{
		EndScene();

		//reset
		s_Renderer2DData.QuadIndexCount = 0;
		s_Renderer2DData.QuadVertexBufferPtr = s_Renderer2DData.QuadVertexBufferBase;
		s_Renderer2DData.TextureSlotIndex = 1;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x,position.y,0.0f }, rotation,size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color)
	{
		EXPLORE_PROFILE_FUNCTION()

		/*
		*@ breif: the way of one quad in one draw call is that using fixed buffer data and setting uniform in shader
		s_Renderer2DData.TextureShader->Bind();
		s_Renderer2DData.TextureShader->SetFloat4("u_color", color);

		//bind white texture, ensure that fragment color is euqal to u_color
		s_Renderer2DData.WhiteTexture->Bind();

		glm::mat4 transform;
		transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, {0.0f,0.0f,1.0f}) * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		s_Renderer2DData.TextureShader->SetMat4("u_ModelMatrix", transform);

		s_Renderer2DData.QuadVertexArray->Bind();
		RenderCommand::DrawIndexd(s_Renderer2DData.QuadVertexArray);
		*/


		//batch rendering: use global position and write color to buffer
		if (s_Renderer2DData.QuadIndexCount >= s_Renderer2DData.MaxIndices || s_Renderer2DData.TextureSlotIndex >= s_Renderer2DData.MaxTextureSlots)
		{
			FlushAndReset();
		}

		const float texIndex = 0.0f; //White Texture
		const float tilingFactor = 1.0f;
		const int quadCount = 4;
		glm::vec2 texCoords[quadCount] = { {0.0f,0.0f},{1.0f,0.0f},{1.0f,1.0f},{0.0f,1.0f} };

		glm::mat4 transform;
		transform = glm::translate(glm::mat4(1.0f), position) 
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f,0.0f,1.0f }) 
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

		for (int index = 0; index < quadCount; index++)
		{
			s_Renderer2DData.QuadVertexBufferPtr->Position = transform * s_Renderer2DData.QuadPosition[index];
			s_Renderer2DData.QuadVertexBufferPtr->Color = color;
			s_Renderer2DData.QuadVertexBufferPtr->TexCoord = texCoords[index];
			s_Renderer2DData.QuadVertexBufferPtr->TexIndex = texIndex;
			s_Renderer2DData.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Renderer2DData.QuadVertexBufferPtr++;
		}

		s_Renderer2DData.QuadIndexCount += 6;
		s_Renderer2DData.Stats.DrawQuads++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		//batch rendering: use global position and write color to buffer
		if (s_Renderer2DData.QuadIndexCount >= s_Renderer2DData.MaxIndices || s_Renderer2DData.TextureSlotIndex >= s_Renderer2DData.MaxTextureSlots)
		{
			FlushAndReset();
		}

		const float texIndex = 0.0f; //White Texture
		const float tilingFactor = 1.0f;
		const int quadCount = 4;
		glm::vec2 texCoords[quadCount] = { {0.0f,0.0f},{1.0f,0.0f},{1.0f,1.0f},{0.0f,1.0f} };

		for (int index = 0; index < quadCount; index++)
		{
			s_Renderer2DData.QuadVertexBufferPtr->Position = transform * s_Renderer2DData.QuadPosition[index];
			s_Renderer2DData.QuadVertexBufferPtr->Color = color;
			s_Renderer2DData.QuadVertexBufferPtr->TexCoord = texCoords[index];
			s_Renderer2DData.QuadVertexBufferPtr->TexIndex = texIndex;
			s_Renderer2DData.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Renderer2DData.QuadVertexBufferPtr++;
		}

		s_Renderer2DData.QuadIndexCount += 6;
		s_Renderer2DData.Stats.DrawQuads++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor)
	{
		DrawQuad({ position.x,position.y,0.0f }, rotation, size, texture, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor)
	{
		EXPLORE_PROFILE_FUNCTION()

		/*
			*@ breif: the way of one quad in one draw call is that using fixed buffer data and setting uniform in shader
		s_Renderer2DData.TextureShader->Bind();
		texture->Bind();
		s_Renderer2DData.TextureShader->SetInt("u_Texture", 0);

		//bind white color, ensure that the fragment color is equal to u_Texture
		s_Renderer2DData.TextureShader->SetFloat4("u_color", { 1.0f,1.0f,1.0f,1.0f });

		glm::mat4 transform;
		transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f,0.0f,1.0f }) * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		s_Renderer2DData.TextureShader->SetMat4("u_ModelMatrix", transform);

		s_Renderer2DData.QuadVertexArray->Bind();
		RenderCommand::DrawIndexd(s_Renderer2DData.QuadVertexArray);
		*/

		//batch rendering: use global position and write color and texIdx to buffer
		if (s_Renderer2DData.QuadIndexCount >= s_Renderer2DData.MaxIndices || s_Renderer2DData.TextureSlotIndex >= s_Renderer2DData.MaxTextureSlots)
		{
			FlushAndReset();
		}

		constexpr glm::vec4 color = { 1.0f,1.0f,1.0f,1.0f };
		const int quadCount = 4;
		glm::vec2 texCoords[quadCount] = { {0.0f,0.0f},{1.0f,0.0f},{1.0f,1.0f},{0.0f,1.0f} };

		float textureIndex = 0.0f;
		for (int i = 1; i < s_Renderer2DData.TextureSlotIndex; i++)
		{
			if (*s_Renderer2DData.Textures[i].get() == *texture.get())
			{
				textureIndex = i;
				break;
			}
		}
		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Renderer2DData.TextureSlotIndex;
			s_Renderer2DData.Textures[s_Renderer2DData.TextureSlotIndex] = texture;
			s_Renderer2DData.TextureSlotIndex++;
		}

		glm::mat4 transform;
		transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f,0.0f,1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

		for (int index = 0; index < quadCount; index++)
		{
			s_Renderer2DData.QuadVertexBufferPtr->Position = transform * s_Renderer2DData.QuadPosition[index];
			s_Renderer2DData.QuadVertexBufferPtr->Color = color;
			s_Renderer2DData.QuadVertexBufferPtr->TexCoord = texCoords[index];
			s_Renderer2DData.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Renderer2DData.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Renderer2DData.QuadVertexBufferPtr++;
		}
		
		s_Renderer2DData.QuadIndexCount += 6;
		s_Renderer2DData.Stats.DrawQuads++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color, const Ref<Texture>& texture, float tilingFactor)
	{
		DrawQuad({ position.x,position.y,0 }, rotation, size, color, texture, tilingFactor);
	}
	
	void Renderer2D::DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color, const Ref<Texture>& texture, float tilingFactor)
	{
		EXPLORE_PROFILE_FUNCTION()

		/*
		 *@ breif: the way of one quad in one draw call is that using fixed buffer data and setting uniform in shader
		s_Renderer2DData.TextureShader->Bind();
		texture->Bind();
		s_Renderer2DData.TextureShader->SetInt("u_Texture", 0);

		s_Renderer2DData.TextureShader->SetFloat4("u_color", color);

		glm::mat4 transform;
		transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f,0.0f,1.0f }) * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		s_Renderer2DData.TextureShader->SetMat4("u_ModelMatrix", transform);

		s_Renderer2DData.QuadVertexArray->Bind();
		RenderCommand::DrawIndexd(s_Renderer2DData.QuadVertexArray);
		*/

		//batch rendering: use global position and write color and texIdx to buffer
		if (s_Renderer2DData.QuadIndexCount >= s_Renderer2DData.MaxIndices || s_Renderer2DData.TextureSlotIndex >= s_Renderer2DData.MaxTextureSlots)
		{
			FlushAndReset();
		}
		
		const int quadCount = 4;
		glm::vec2 texCoords[quadCount] = { {0.0f,0.0f},{1.0f,0.0f},{1.0f,1.0f},{0.0f,1.0f} };

		float textureIndex = 0.0f;
		for (int i = 1; i < s_Renderer2DData.TextureSlotIndex; i++)
		{
			if (*s_Renderer2DData.Textures[i].get() == *texture.get())
			{
				textureIndex = i;
				break;
			}
		}
		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Renderer2DData.TextureSlotIndex;
			s_Renderer2DData.Textures[s_Renderer2DData.TextureSlotIndex] = texture;
			s_Renderer2DData.TextureSlotIndex++;
		}

		glm::mat4 transform;
		transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f,0.0f,1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

		for (int index = 0; index < quadCount; index++)
		{
			s_Renderer2DData.QuadVertexBufferPtr->Position = transform * s_Renderer2DData.QuadPosition[index];
			s_Renderer2DData.QuadVertexBufferPtr->Color = color;
			s_Renderer2DData.QuadVertexBufferPtr->TexCoord = texCoords[index];
			s_Renderer2DData.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Renderer2DData.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Renderer2DData.QuadVertexBufferPtr++;
		}

		s_Renderer2DData.QuadIndexCount += 6;
		s_Renderer2DData.Stats.DrawQuads++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color, const Ref<SubTexture>& subTexture, float tilingFactor)
	{
		EXPLORE_PROFILE_FUNCTION()

		DrawQuad({ position.x,position.y,0.0f }, rotation, size, color,subTexture, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color, const Ref<SubTexture>& subTexture, float tilingFactor)
	{
		EXPLORE_PROFILE_FUNCTION()

		//batch rendering: use global position and write color and texIdx to buffer
		if (s_Renderer2DData.QuadIndexCount >= s_Renderer2DData.MaxIndices || s_Renderer2DData.TextureSlotIndex >= s_Renderer2DData.MaxTextureSlots)
		{
			FlushAndReset();
		}

		const int quadCount = 4;
		Ref<Texture> texture = subTexture->GetTexture();
		std::vector<glm::vec2> texCoords = subTexture->GetCoods();

		float textureIndex = 0.0f;
		for (int i = 1; i < s_Renderer2DData.TextureSlotIndex; i++)
		{
			if (*s_Renderer2DData.Textures[i].get() == *texture.get())
			{
				textureIndex = i;
				break;
			}
		}
		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Renderer2DData.TextureSlotIndex;
			s_Renderer2DData.Textures[s_Renderer2DData.TextureSlotIndex] = texture;
			s_Renderer2DData.TextureSlotIndex++;
		}

		glm::mat4 transform;
		transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f,0.0f,1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

		for (int index = 0; index < quadCount; index++)
		{
			s_Renderer2DData.QuadVertexBufferPtr->Position = transform * s_Renderer2DData.QuadPosition[index];
			s_Renderer2DData.QuadVertexBufferPtr->Color = color;
			s_Renderer2DData.QuadVertexBufferPtr->TexCoord = texCoords[index];
			s_Renderer2DData.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Renderer2DData.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Renderer2DData.QuadVertexBufferPtr++;
		}

		s_Renderer2DData.QuadIndexCount += 6;
		s_Renderer2DData.Stats.DrawQuads++;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Renderer2DData.Stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Renderer2DData.Stats;
	}
}