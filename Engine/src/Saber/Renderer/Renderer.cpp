#include "Renderer.h"
#include "Renderer2D.h"
#include "Renderer3D.h"
#include "Saber/pch.h"

#include "Platform/OpenGl/OpenGLShader.h"

namespace Saber
{
Renderer::SceneData *Renderer::s_SceneData = new Renderer::SceneData;

void Renderer::Init()
{
    RenderCommand::Init();
    // Renderer2D::Init();
    Renderer3D::Init();
}

void Renderer::Shutdown() { Renderer2D::Shutdown(); }

void Renderer::OnWindowResize(uint32_t width, uint32_t height) { RenderCommand::SetViewport(0, 0, width, height); }

void Renderer::BeginScene(OrthographicCamera &camera)
{
    s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene() {}

void Renderer::Submit(const Ref<Shader> &shader, const Ref<VertexArray> &vertexArray, const glm::mat4 &transform)
{
    shader->Bind();
    shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
    shader->SetMat4("u_Transform", transform);

    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
}
} // namespace Saber
