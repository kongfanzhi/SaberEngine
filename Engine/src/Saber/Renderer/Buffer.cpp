#include "Buffer.h"
#include "Saber/pch.h"

#include "Platform/OpenGl/OpenGLBuffer.h"
#include "Renderer.h"

namespace Saber
{
Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        SB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLVertexBuffer>(size);
    }

    SB_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

Ref<VertexBuffer> VertexBuffer::Create(float *vertices, uint32_t size)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        SB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLVertexBuffer>(vertices, size);
    }
    SB_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

Ref<IndexBuffer> IndexBuffer::Create(uint32_t *indices, uint32_t size)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        SB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLIndexBuffer>(indices, size);
    }

    SB_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}
} // namespace Saber
