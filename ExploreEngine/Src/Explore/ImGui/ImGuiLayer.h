#pragma once
#include "Explore/Core/Layer.h"
#include "Explore/Events/KeyEvent.h"
#include "Explore/Events/MouseEvent.h"
#include "Explore/Events/ApplicationEvent.h"

namespace Explore
{
	//ImGuiLayer is used to debug draw
	class EXPLORE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();

		~ImGuiLayer();

		virtual void OnAttach() override;

		virtual void OnDetach() override;

		virtual void OnImGuiRender() override;

		void Begin();

		void End();

	private:
		float m_Time = 0.0f;
	};
}