#pragma once
#include "Explore/input.h"

namespace Explore
{
	class WindowsInput : public Input
	{
		virtual bool IsKeyPressedImpl(int keycode);

		virtual bool IsMouseButtonPressedImpl(int button);

		virtual std::pair<float, float> GetMousePositionImpl();

		virtual float GetMouseXImpl();

		virtual float GetMouseYImpl();
	};
}