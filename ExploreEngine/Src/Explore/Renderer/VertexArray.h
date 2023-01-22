#pragma once
#include "Buffer.h"

namespace Explore
{
	//A Array which Contains References of Buffers
	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() = 0;

		virtual void UnBind() = 0;

		virtual void AddVertexBuffer(Ref<VertexBuffer>) = 0;

		virtual std::vector<Ref<VertexBuffer>>& GetVertexBuffer() = 0;

		virtual void SetIndexBuffer(Ref<IndexBuffer>) = 0;

		virtual Ref<IndexBuffer>& GetIndexBuffer() = 0; 

		static Ref<VertexArray> Create();
	};
}