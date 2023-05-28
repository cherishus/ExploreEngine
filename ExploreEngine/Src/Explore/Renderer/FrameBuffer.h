#pragma once

namespace Explore
{
	enum class FrameBufferTextureFormat
	{
		None = 0,

		//color
		RGBA8,
		RGBA16F,

		//depth/stencil
		DEPTH24STENCIL8,
		Depth = DEPTH24STENCIL8
	};

	struct FrameBufferTextureSpecification
	{
		FrameBufferTextureSpecification() = default;
		FrameBufferTextureSpecification(FrameBufferTextureFormat format)
			: Format(format)
		{}

		FrameBufferTextureFormat Format;
		//ToDo: Filter Wrapping
	};

	struct FrameBufferAttachmentSpecification
	{
		FrameBufferAttachmentSpecification() = default;
		FrameBufferAttachmentSpecification(std::initializer_list<FrameBufferTextureSpecification> attachments)
			: Attachments(attachments)
		{}

		std::vector<FrameBufferTextureSpecification> Attachments;
	};

	struct FrameBufferSpecification
	{
		uint32_t Width, Height;
		FrameBufferAttachmentSpecification Attachment; //attachments which can be defined by user
		uint32_t Sample = 1;
		bool SwapChainTarget = false;
	};

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;

		virtual void Bind() = 0;

		virtual void UnBind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual FrameBufferSpecification& GetSpec() = 0;

		virtual uint32_t GetColorAttachmentRenderId(int index = 0) = 0;
		
		static Ref<FrameBuffer> Create(const FrameBufferSpecification& Spec);
	};
}