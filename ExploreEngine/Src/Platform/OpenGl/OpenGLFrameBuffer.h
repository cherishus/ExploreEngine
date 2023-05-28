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

		virtual uint32_t GetColorAttachmentRenderId(int index = 0) override { return m_ColorAttachmentIds[index]; }

	private:
		void InValidate();

	private:
		uint32_t m_RendererId = 0;

		std::vector<uint32_t> m_ColorAttachmentIds;

		uint32_t m_DepthAttachmentId;
		
		FrameBufferSpecification m_Spec;

		std::vector<FrameBufferTextureSpecification> m_ColorTextureSpecifications;

		FrameBufferTextureSpecification m_DepthTextureSpecification = FrameBufferTextureFormat::None;
	};
}