#include "Shader.h"
#include "Saber/pch.h"

#include "Platform/OpenGl/OpenGLShader.h"
#include "Renderer.h"

namespace Saber
{
Ref<Shader> Shader::Create(const std::string &filepath)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        SB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLShader>(filepath);

        SB_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
    return nullptr;
}

Ref<Shader> Shader::Create(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        SB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
    }

    SB_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

void ShaderLibrary::Add(const std::string &name, const Ref<Shader> &shader)
{
    SB_CORE_ASSERT(!Exists(name), "Shader already exists!");
    m_Shaders[name] = shader;
}

void ShaderLibrary::Add(const Ref<Shader> &shader)
{
    auto &name = shader->GetName();
    Add(name, shader);
}

Ref<Shader> ShaderLibrary::Load(const std::string &filepath)
{
    auto shader = Shader::Create(filepath);
    Add(shader);
    return shader;
}

Saber::Ref<Saber::Shader> ShaderLibrary::Load(const std::string &name, const std::string &filepath)
{
    auto shader = Shader::Create(filepath);
    Add(name, shader);
    return shader;
}

Saber::Ref<Saber::Shader> ShaderLibrary::Get(const std::string &name)
{
    SB_CORE_ASSERT(Exists(name), "Shader not found!");
    return m_Shaders[name];
}

bool ShaderLibrary::Exists(const std::string &name) const { return m_Shaders.find(name) != m_Shaders.end(); }
} // namespace Saber
