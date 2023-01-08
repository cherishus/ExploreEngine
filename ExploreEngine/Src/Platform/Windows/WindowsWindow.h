#pragma once
#include "Explore/Window.h"
#include "Explore/Renderer/GraphicsContext.h"
#include "GLFW/glfw3.h"

namespace Explore
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);

		virtual ~WindowsWindow();

		void OnUpdate() override;

		uint32_t GetWidth() override { return m_Data.Width; }

		uint32_t  GetHeight() override { return m_Data.Height; }

		// Window attributes
		void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		
		void SetVSync(bool enabled) override;

		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() override { return m_Window; }

	private:
		void Init(const WindowProps& props);

		void Shutdown();

	private:
		GLFWwindow* m_Window; //use GLFW to implement window class in window platform

		Ref<GraphicsContext> m_Context; //rendering context

		struct WindowData
		{
			std::string Title;

			unsigned int Width, Height;

			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}