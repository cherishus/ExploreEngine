#pragma once
#include "Explore/Renderer/Buffer.h"

namespace Explore
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);

		virtual ~OpenGLVertexBuffer();

		virtual void Bind() override;

		virtual void UnBind() override;

		virtual void SetLayout(BufferLayout layout) override;

		virtual BufferLayout& GetLayout() override;

	private:
		uint32_t m_VertexBufferID; //VBO Id

		BufferLayout m_Layout; //data layout in vertex buffer
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);

		virtual ~OpenGLIndexBuffer();

		virtual void Bind() override;

		virtual void UnBind() override;

		virtual uint32_t GetCount() override { return m_Count; }

	private:
		uint32_t m_IndexBufferID; //EBO Id

		uint32_t m_Count; //indices count
	};
}