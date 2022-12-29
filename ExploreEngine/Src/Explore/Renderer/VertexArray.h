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

		virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer>) = 0;

		virtual std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffer() = 0;

		virtual void SetIndexBuffer(std::shared_ptr<IndexBuffer>) = 0;

		virtual std::shared_ptr<IndexBuffer>& GetIndexBuffer() = 0; 

		static VertexArray* Create();
	};
}