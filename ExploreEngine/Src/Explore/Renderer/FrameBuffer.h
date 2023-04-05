#pragma once

namespace Explore
{
	struct FrameBufferSpecification
	{
		uint32_t Width, Height;
		uint32_t Sample = 1;
		bool SwapChainTarget;
	};

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;

		virtual void Bind() = 0;

		virtual void UnBind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual FrameBufferSpecification& GetSpec() = 0;

		virtual uint32_t GetColorAttachmentRenderId() = 0;
		
		static Ref<FrameBuffer> Create(const FrameBufferSpecification& Spec);
	};
}