#pragma once
#include "Explore/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Explore
{
	/*
	fuction: receive window handle and
	         set this as render target or context for rendering api
	*/
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;

		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle; //Window Handle
	};
}