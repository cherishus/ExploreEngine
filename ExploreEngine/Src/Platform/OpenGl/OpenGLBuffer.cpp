#include "enginepch.h"
#include "OpenGLBuffer.h"
#include "glad/glad.h"

namespace Explore
{
	//--------------------------Vertex Buffer----------------------------------------------------
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		EXPLORE_PROFILE_FUNCTION()
		glGenBuffers(1, &m_VertexBufferID);
		//bind VBO
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
		//move data to GPU cache
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		EXPLORE_PROFILE_FUNCTION()
		glDeleteBuffers(1, &m_VertexBufferID);
	}

	void OpenGLVertexBuffer::Bind()
	{
		EXPLORE_PROFILE_FUNCTION()
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
	}

	void OpenGLVertexBuffer::UnBind()
	{
		EXPLORE_PROFILE_FUNCTION()
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetLayout(BufferLayout layout)
	{
		m_Layout = layout;
	}

	BufferLayout& OpenGLVertexBuffer::GetLayout()
	{
		return m_Layout;
	}

	//--------------------------Index Buffer----------------------------------------------------
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : m_Count(count)
	{
		EXPLORE_PROFILE_FUNCTION()
		glGenBuffers(1, &m_IndexBufferID);
		//bind EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		EXPLORE_PROFILE_FUNCTION()
		glDeleteBuffers(1, &m_IndexBufferID);
	}

	void OpenGLIndexBuffer::Bind()
	{
		EXPLORE_PROFILE_FUNCTION()
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
	}

	void OpenGLIndexBuffer::UnBind()
	{
		EXPLORE_PROFILE_FUNCTION()
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}