#pragma once
#include "Event.h"
#include <string>
#include <sstream>

namespace Explore
{
	class EXPLORE_API KeyEvent : public Event
	{
	public:
		inline int GetKeycode() { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryKeyboard | EventCategory::EventCategoryInput)
	
	protected:
		KeyEvent(int Keycode) : m_KeyCode(Keycode) {}

		int m_KeyCode;
	};

	class EXPLORE_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int Keycode, int repeatCount) : KeyEvent(Keycode),m_RepeatCount(repeatCount) {}

		inline int GetRepeatCount() { return m_RepeatCount; }

		std::string ToString()
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << "(" << m_RepeatCount << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int m_RepeatCount;
	};

	class EXPLORE_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int Keycode) : KeyEvent(Keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class EXPLORE_API KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode) : KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}