#include "Saber/Renderer/GraphicsContext.h"
#include "Saber/pch.h"

#include "Platform/OpenGL/OpenGLContext.h"
// #include "Saber/Renderer/Renderer.h"

namespace Saber
{

Scope<GraphicsContext> GraphicsContext::Create(void *window)
{
    /* switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        SB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return CreateScope<OpenGLContext>(static_cast<GLFWwindow *>(window));
    } */

    return CreateScope<OpenGLContext>(static_cast<GLFWwindow *>(window));

    SB_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

} // namespace Saber