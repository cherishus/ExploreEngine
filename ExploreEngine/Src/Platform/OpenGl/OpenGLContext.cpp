#include "enginepch.h"
#include "OpenGLContext.h"
#include "Explore/Log.h"
#include "Explore/Core.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace Explore
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle):m_WindowHandle(windowHandle)
	{
		EXPLORE_ASSERT(m_WindowHandle, "Window Handle is nullptr!");
	}

	void OpenGLContext::Init()
	{
		//make context for openGL
		glfwMakeContextCurrent(m_WindowHandle);
		//init glad
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		EXPLORE_ASSERT(status, "Failed to initialize Glad!");
		//log info
		char* vendor = (char*)glGetString(GL_VENDOR);
		EXPLORE_CORE_LOG(info, "Vendor: {0}", vendor);
		char* render = (char*)glGetString(GL_RENDERER);
		EXPLORE_CORE_LOG(info, "Render: {0}", render);
		char* version = (char*)glGetString(GL_VERSION);
		EXPLORE_CORE_LOG(info, "Version: {0}",version );
	}

	void OpenGLContext::SwapBuffers()
	{
		//swap buffer for openGL
		glfwSwapBuffers(m_WindowHandle);
	}
}