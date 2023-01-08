#include "enginepch.h"
#include "RenderCommand.h"

namespace Explore
{
	Ref<RendererAPI> RenderCommand::m_RendererAPI = Ref<RendererAPI>(RendererAPI::Create());
}