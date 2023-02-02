#include "enginepch.h"
#include "OpenGLVertexArray.h"
#include "glad/glad.h"

namespace Explore
{
	static GLenum ConvertShaderDataTypeToOpenGLDataType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::None:		return GL_NONE;
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4:
		case ShaderDataType::Mat3:
		case ShaderDataType::Mat4:		return GL_FLOAT;
		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4:		return GL_INT;
		case ShaderDataType::Bool:		return GL_BOOL;
		}
		EXPLORE_ASSERT(false, "UnSupported ShaderData Type");
		return GL_NONE;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		EXPLORE_PROFILE_FUNCTION()
		glGenVertexArrays(1, &m_VertexArrayID);
		glBindVertexArray(m_VertexArrayID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		EXPLORE_PROFILE_FUNCTION()
		glDeleteVertexArrays(1, &m_VertexArrayID);
	}

	void OpenGLVertexArray::Bind()
	{
		EXPLORE_PROFILE_FUNCTION()
		glBindVertexArray(m_VertexArrayID);
	}

	void OpenGLVertexArray::UnBind()
	{
		EXPLORE_PROFILE_FUNCTION()
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(Ref<VertexBuffer> vertexBuffer)
	{
		EXPLORE_PROFILE_FUNCTION()
		//Add reference of vertexBuffer to VAO
		glBindVertexArray(m_VertexArrayID);
		vertexBuffer->Bind();

		//set vertex attribute pointer by layout
		uint32_t index = 0;
		BufferLayout layout = vertexBuffer->GetLayout();
		EXPLORE_ASSERT(!layout.GetBufferElements().empty(), "VertexBuffer Layout is Empty!");
		for (const auto& elem : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,							//layout or index
				GetShaderDataTypeComponentCount(elem.type),			//component count
				ConvertShaderDataTypeToOpenGLDataType(elem.type),	//type
				elem.normalize ? GL_TRUE : GL_FALSE,				//normalize
				layout.GetLayoutStride(),							//stride
				(const void*)elem.offset);							//offset
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(Ref<IndexBuffer> indexBuffer)
	{
		EXPLORE_PROFILE_FUNCTION()
		glBindVertexArray(m_VertexArrayID);
		indexBuffer->Bind(); //Add reference of indexBuffer to VAO
		
		m_IndexBuffer = indexBuffer;
	}
}