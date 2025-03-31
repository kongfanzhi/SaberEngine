#include "Saber/Renderer/RenderCommand.h"
#include "Saber/pch.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Saber
{
Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}
