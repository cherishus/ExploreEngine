#pragma once
#include "Explore/Renderer/FrameBuffer.h"

namespace Explore
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);

		~OpenGLFrameBuffer();

		virtual void Bind() override;

		virtual void UnBind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual FrameBufferSpecification& GetSpec() override { return m_Spec; }

		virtual uint32_t GetColorAttachmentRenderId() override { return m_ColorAttachmentId; }

	private:
		void InValidate();

	private:
		uint32_t m_RendererId = 0;

		uint32_t m_ColorAttachmentId = 0, m_DepthAttachmentId = 0;

		FrameBufferSpecification m_Spec;
	};
}