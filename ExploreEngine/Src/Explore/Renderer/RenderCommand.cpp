#include "enginepch.h"
#include "RenderCommand.h"

namespace Explore
{
	std::shared_ptr<RendererAPI> RenderCommand::m_RendererAPI = std::shared_ptr<RendererAPI>(RendererAPI::Create());

}