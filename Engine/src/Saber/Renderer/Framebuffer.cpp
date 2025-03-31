#include "Framebuffer.h"
#include "Saber/pch.h"

#include "Platform/OpenGl/OpenGLFramebuffer.h"
#include "RendererAPI.h"

namespace Saber
{
Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification &spec)
{
    switch (RendererAPI::GetAPI())
    {
    case RendererAPI::API::None:
        SB_CORE_ASSERT(false, "RendererAPI::None is not supported");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLFramebuffer>(spec);
    }

    SB_CORE_ASSERT(false, "RendererAPI::None is not supported");
    return nullptr;
}
} // namespace Saber
