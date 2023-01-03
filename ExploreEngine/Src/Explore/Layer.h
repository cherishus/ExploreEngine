#pragma once
#include <string>
#include "Events/Event.h"
#include "Explore/Core/Timestep.h"

namespace Explore
{
	class EXPLORE_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");

		virtual ~Layer();

		virtual void OnAttach() {}

		virtual void OnDetach() {}

		virtual void OnUpdate(Timestep ts) {}

		virtual void OnImGuiRender() {}

		virtual void OnEvent(Event& event) {}

	protected:
		std::string m_Debugname;
	};
}