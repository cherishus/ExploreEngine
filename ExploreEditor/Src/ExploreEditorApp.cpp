#include "Explore.h"
#include "Explore/Core/Entrypoint.h"
#include "EditorLayer.h"

namespace Explore
{
	class ExploreEditor : public Application
	{
	public:
		ExploreEditor() : Application("Explore Editor")
		{
			PushLayer(new EditorLayer());
		}

		~ExploreEditor() {}
	};

	Application* CreateApplication()
	{
		return new ExploreEditor();
	}
}