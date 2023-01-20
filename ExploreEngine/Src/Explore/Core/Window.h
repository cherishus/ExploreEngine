#pragma once
#include "Core.h"
#include "Explore/Events/Event.h"
#include <functional>

namespace Explore
{
	struct WindowProps
	{
		std::string Title;

		uint32_t Width;

		uint32_t Height;

		WindowProps(const std::string& title = "Explore Engine", uint32_t width = 1600, uint32_t height = 900) : Title(title),Width(width),Height(height) {}
	};


	//Window Interface cross platform
	class EXPLORE_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() = 0;

		virtual uint32_t GetHeight() = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

		virtual void SetVSync(bool enabled) = 0;

		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}