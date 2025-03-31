#include "RendererAPI.h"
#include "Saber/pch.h"

#include "Platform/OpenGl/OpenGLRendererAPI.h"

namespace Saber
{
RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

Scope<RendererAPI> RendererAPI::Create()
{
    switch (s_API)
    {
    case RendererAPI::API::None:
        SB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return CreateScope<OpenGLRendererAPI>();
    }

    SB_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

} // namespace Saber
