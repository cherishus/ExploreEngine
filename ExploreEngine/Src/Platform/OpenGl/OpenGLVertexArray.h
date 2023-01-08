#pragma once
#include "Explore/Renderer/VertexArray.h"

namespace Explore
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();

		virtual ~OpenGLVertexArray();

		virtual void Bind() override;

		virtual void UnBind() override;

		virtual void AddVertexBuffer(Ref<VertexBuffer> vertexBuffer) override;

		virtual std::vector<Ref<VertexBuffer>>& GetVertexBuffer() override { return m_VertexBuffers; }

		virtual void SetIndexBuffer(Ref<IndexBuffer> indexBuffer) override;

		virtual Ref<IndexBuffer>& GetIndexBuffer() override { return m_IndexBuffer; }
		
	private:
		uint32_t m_VertexArrayID; //VAO Id

		std::vector<Ref<VertexBuffer>> m_VertexBuffers;

		Ref<IndexBuffer> m_IndexBuffer;
	};
}