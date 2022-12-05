#include "enginepch.h"
#include "WindowsInput.h"
#include "Explore/Application.h"
#include "GLFW/glfw3.h"

namespace Explore
{
	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		glfwGetKey(window, keycode);
		return keycode == GLFW_PRESS || keycode == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		glfwGetMouseButton(window, button);
		return button == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return {(float)xPos,(float)yPos};
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto Pos = GetMousePosition();
		return (float)Pos.first;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto Pos = GetMousePosition();
		return (float)Pos.second;
	}
}