#include "Texture.h"
#include "Saber/pch.h"

#include "Platform/OpenGl/OpenGLTexture.h"
#include "Renderer.h"
#include "RendererAPI.h"

namespace Saber
{
Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        SB_CORE_ASSERT(false, "RendererAPI::None is not supported");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLTexture2D>(width, height);
    }

    SB_CORE_ASSERT(false, "Unknown RendererAPI");
    return nullptr;
}

Ref<Texture2D> Texture2D::Create(const std::string &filepath)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        SB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLTexture2D>(filepath);
    }

    SB_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}
} // namespace Saber
