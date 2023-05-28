#include "enginepch.h"
#include "OpenGLFrameBuffer.h"
#include "glad/glad.h"

namespace Explore
{
	namespace Utils
	{
		static GLenum TextureTarget(bool bMultiSampled)
		{
			return bMultiSampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(bool bMultiSampled, uint32_t* data, uint32_t count)
		{
			glCreateTextures(TextureTarget(bMultiSampled), count, data);
		}

		static void BindTexture(bool bMultiSampled, uint32_t id)
		{
			glBindTexture(TextureTarget(bMultiSampled), id);
		}

		static void AttachColorTextures(uint32_t id, int samples, GLenum format, uint32_t width, uint32_t height, uint32_t index)
		{
			bool bMultiSampled = samples > 1;
			if (bMultiSampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(bMultiSampled), id, 0);
		}

		static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachment,uint32_t width, uint32_t height)
		{
			bool bMultiSampled = samples > 1;
			if (bMultiSampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, TextureTarget(bMultiSampled), id, 0);
		}

		static bool IsDepthFormat(FrameBufferTextureFormat format)
		{
			switch (format)
			{
				case FrameBufferTextureFormat::DEPTH24STENCIL8: return true; break;
			}
			return false;
		}
	}

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
		: m_Spec(spec)
	{
		//sort
		for (const auto& texturespec : spec.Attachment.Attachments)
		{
			if (!Utils::IsDepthFormat(texturespec.Format))
			{
				m_ColorTextureSpecifications.emplace_back(texturespec);
			}
			else
			{
				m_DepthTextureSpecification = texturespec;
			}
		}
		InValidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_RendererId);
		glDeleteTextures(m_ColorAttachmentIds.size(), m_ColorAttachmentIds.data());
		glDeleteTextures(1, &m_DepthAttachmentId);
	}

	void OpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);
		glViewport(0, 0, m_Spec.Width, m_Spec.Height);
	}

	void OpenGLFrameBuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		m_Spec.Width = width;
		m_Spec.Height = height;
		InValidate();
	}

	void OpenGLFrameBuffer::InValidate()
	{
		if (m_RendererId > 0)
		{
			glDeleteFramebuffers(1, &m_RendererId);
			glDeleteTextures(m_ColorAttachmentIds.size(), m_ColorAttachmentIds.data());
			glDeleteTextures(1, &m_DepthAttachmentId);
			m_ColorAttachmentIds.clear();
			m_DepthAttachmentId = 0;
		}

		glGenFramebuffers(1, &m_RendererId);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);

		//Attachments
		bool bMultiSampled = m_Spec.Sample > 1;
		if (m_ColorTextureSpecifications.size() > 0)
		{
			m_ColorAttachmentIds.resize(m_ColorTextureSpecifications.size());
			Utils::CreateTextures(bMultiSampled, m_ColorAttachmentIds.data(), m_ColorAttachmentIds.size());
			for (uint32_t index = 0; index < m_ColorAttachmentIds.size(); index++)
			{
				Utils::BindTexture(bMultiSampled, m_ColorAttachmentIds[index]);
				switch (m_ColorTextureSpecifications[index].Format)
				{
					case FrameBufferTextureFormat::RGBA8: 
						Utils::AttachColorTextures(m_ColorAttachmentIds[index], m_Spec.Sample, GL_RGBA8, m_Spec.Width, m_Spec.Height, index);
						break;
				}
			}
		}

		if (m_DepthTextureSpecification.Format != FrameBufferTextureFormat::None)
		{
			Utils::CreateTextures(bMultiSampled, &m_DepthAttachmentId, 1);
			Utils::BindTexture(bMultiSampled, m_DepthAttachmentId);
			switch (m_DepthTextureSpecification.Format)
			{
			case FrameBufferTextureFormat::DEPTH24STENCIL8:
				Utils::AttachDepthTexture(m_DepthAttachmentId, m_Spec.Sample, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Spec.Width, m_Spec.Height);
				break;
			}
		}

		if (m_ColorAttachmentIds.size() > 1)
		{
			EXPLORE_ASSERT(m_ColorAttachmentIds.size() <= 4);
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColorAttachmentIds.size(), buffers);
		}
		else if (m_ColorAttachmentIds.empty())
		{
			glDrawBuffer(GL_NONE);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}