#pragma once
#include "Explore/Core/Log.h"

namespace Explore
{
	//data type in shader
	enum class ShaderDataType : uint8_t
	{
		None = 0,
		Float,Float2,Float3,Float4,
		Mat3,Mat4,
		Int,Int2,Int3,Int4,
		Bool
	};

	static uint32_t GetShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::None:      return 0;
			case ShaderDataType::Float:		return 4;
			case ShaderDataType::Float2:	return 4 * 2;
			case ShaderDataType::Float3:	return 4 * 3;
			case ShaderDataType::Float4:	return 4 * 4;
			case ShaderDataType::Mat3:		return 4 * 3 * 3;
			case ShaderDataType::Mat4:		return 4 * 4 * 4;
			case ShaderDataType::Int:		return 4;
			case ShaderDataType::Int2:		return 4 * 2;
			case ShaderDataType::Int3:		return 4 * 3;
			case ShaderDataType::Int4:		return 4 * 4;
			case ShaderDataType::Bool:		return 1;
		}
		EXPLORE_ASSERT(false, "UnSupported ShaderData Type");
		return 0;
	}

	static uint32_t GetShaderDataTypeComponentCount(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::None:		return 0;
			case ShaderDataType::Float:		return 1;
			case ShaderDataType::Float2:	return 2;
			case ShaderDataType::Float3:	return 3;
			case ShaderDataType::Float4:	return 4;
			case ShaderDataType::Mat3:		return 3 * 3;
			case ShaderDataType::Mat4:		return 4 * 4;
			case ShaderDataType::Int:		return 1;
			case ShaderDataType::Int2:		return 2;
			case ShaderDataType::Int3:		return 3;
			case ShaderDataType::Int4:		return 4;
			case ShaderDataType::Bool:		return 1;
		}
		EXPLORE_ASSERT(false, "UnSupported ShaderData Type");
		return 0;
	}

	struct BufferElement
	{
		std::string name;
		ShaderDataType type;
		uint32_t size;
		uint32_t offset; //equals to size of all elements before this element
		bool normalize;

		BufferElement() = default;

		BufferElement(std::string inName,ShaderDataType inType,bool inNormalize = false) : name(inName),type(inType),size(GetShaderDataTypeSize(inType)),offset(0),normalize(inNormalize) {}
	};
	
	//describe data layout for buffer
	class BufferLayout
	{
	public:
		BufferLayout() = default;

		BufferLayout(const std::initializer_list<BufferElement>& elements) : m_Elements(elements)
		{
			CalculateOffset();
		}

		inline std::vector<BufferElement>& GetBufferElements() { return m_Elements; }

		//To support for loop
		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }

		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
	
		uint32_t GetLayoutStride() { return m_Stride; }

	private:
		void CalculateOffset()
		{
			uint32_t offset = 0; //the whole size of all elements before the specific element 
			m_Stride = 0;
			for (auto& elem : m_Elements)
			{
				elem.offset = offset;
				offset += elem.size;
				m_Stride += elem.size;
			}
		}

	private:
		std::vector<BufferElement> m_Elements;

		uint32_t m_Stride; //the whole size for layout
	};
	
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() = 0;

		virtual void UnBind() = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;

		virtual void SetLayout(BufferLayout layout) = 0;

		virtual BufferLayout& GetLayout() = 0;

		static Ref<VertexBuffer> Create(uint32_t size);

		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() = 0;

		virtual void UnBind() = 0;

		virtual uint32_t GetCount() = 0;

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
	};
}