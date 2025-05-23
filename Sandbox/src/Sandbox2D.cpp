#include "Sandbox2D.h"

#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGl/OpenGLShader.h"
#include "imgui/imgui.h"

#include "Saber/Renderer/Renderer2D.h"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f) {}

void Sandbox2D::OnAttach()
{
    SB_PROFILE_FUNCTION();

    m_CheckerboardTexture = Saber::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach() { SB_PROFILE_FUNCTION(); }

void Sandbox2D::OnUpdate(Saber::Timestep ts)
{
    SB_PROFILE_FUNCTION();

    m_CameraController.OnUpdate(ts);

    // Render
    Saber::Renderer2D::ResetStats();
    {
        SB_PROFILE_SCOPE("Renderer Prep");
        Saber::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        Saber::RenderCommand::Clear();
    }

    {
        static float rotation = 0.0f;
        rotation += ts * 50.0f;

        SB_PROFILE_SCOPE("Renderer Draw");
        Saber::Renderer2D::BeginScene(m_CameraController.GetCamera());
        Saber::Renderer2D::DrawRotatedQuad({1.0f, 0.0f}, {0.8f, 0.8f}, -45.0f, {0.8f, 0.2f, 0.3f, 1.0f});
        Saber::Renderer2D::DrawQuad({-1.0f, 0.0f}, {0.8f, 0.8f}, {0.8f, 0.2f, 0.3f, 1.0f});
        Saber::Renderer2D::DrawQuad({0.5f, -0.5f}, {0.5f, 0.75f}, {0.2f, 0.3f, 0.8f, 1.0f});
        Saber::Renderer2D::DrawQuad({0.0f, 0.0f, -0.1f}, {20.0f, 20.0f}, m_CheckerboardTexture, 10.0f);
        Saber::Renderer2D::DrawRotatedQuad({-2.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, rotation, m_CheckerboardTexture, 20.0f);
        Saber::Renderer2D::EndScene();

        Saber::Renderer2D::BeginScene(m_CameraController.GetCamera());
        for (float y = -5.0f; y < 5.0f; y += 0.5f)
        {
            for (float x = -5.0f; x < 5.0f; x += 0.5f)
            {
                glm::vec4 color = {(x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f};
                Saber::Renderer2D::DrawQuad({x, y}, {0.45f, 0.45f}, color);
            }
        }
        Saber::Renderer2D::EndScene();
    }
}

void Sandbox2D::OnImGuiRender()
{
    SB_PROFILE_FUNCTION();

    ImGui::Begin("Settings");

    auto stats = Saber::Renderer2D::GetStats();
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void Sandbox2D::OnEvent(Saber::Event &e) { m_CameraController.OnEvent(e); }
