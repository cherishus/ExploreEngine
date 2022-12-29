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

		virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer) override;

		virtual std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffer() override { return m_VertexBuffers; }

		virtual void SetIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer) override;

		virtual std::shared_ptr<IndexBuffer>& GetIndexBuffer() override { return m_IndexBuffer; }
		
	private:
		uint32_t m_VertexArrayID; //VAO Id

		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;

		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};
}