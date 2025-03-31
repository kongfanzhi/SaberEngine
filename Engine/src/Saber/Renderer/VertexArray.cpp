#include "VertexArray.h"
#include "Saber/pch.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Renderer.h"

namespace Saber
{
Ref<VertexArray> VertexArray::Create()
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        SB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLVertexArray>();
    }

    SB_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}
} // namespace Saber
